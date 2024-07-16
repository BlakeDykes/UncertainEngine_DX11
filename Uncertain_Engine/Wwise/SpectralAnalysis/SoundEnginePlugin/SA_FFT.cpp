#include "SA_FFT.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <ios>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cmath>

AkReal32 FFT::NORM_FACTOR = 1.0f / (SA_Real)FREQ_RESOLUTION;

FFT::FFT()
	: ChannelCount(0)
    , poWriteBuffer(nullptr)
    , SampleRate(0)
    , PendingFrameTotal(0)
{
#if (FFT_LOGS == 1)
    InitPrinters();
#endif // FFT_LOGS
}

void FFT::Setup(AK::IAkPluginMemAlloc* in_pAllocator, AkAudioFormat& in_rFormat)
{
	SampleRate = in_rFormat.uSampleRate;
    ChannelCount = (AkUInt8)in_rFormat.GetNumChannels();

    poWriteBuffer = AK_PLUGIN_NEW(in_pAllocator, SpectralDAO);
    poWriteBuffer->Init_AkAlloc(in_pAllocator);

    ClearBuffers();

#if (FFT_LOGS == 1)
    SetupPrinters(*in_pAllocator, in_rFormat);
#endif // FFT_LOGS
}

void FFT::Destroy(AK::IAkPluginMemAlloc* in_pAllocator)
{
#if (FFT_LOGS == 1)
    DestroyAndWritePrinters(in_pAllocator);
#endif // FFT_LOGS

    poWriteBuffer->Destroy_AkAlloc(in_pAllocator);
    AK_PLUGIN_DELETE(in_pAllocator, poWriteBuffer);
}

FFT::FrameStatus FFT::Execute(AkAudioBuffer* io_pBuffer)
{
    if (!io_pBuffer->HasData()) return FrameStatus::Incomplete;
    
    FrameStatus res = FrameStatus::Incomplete;

    io_pBuffer->ZeroPadToMaxFrames();

#if (FFT_LOGS == 1)
    PreExecutePrinters(numChannels);
#endif // FFT_LOGS

    AkUInt16 framesPerChannel = io_pBuffer->uValidFrames / ChannelCount;

    for (AkUInt8 i = 0; i < ChannelCount; ++i)
    {
        // Get buffer data for channel index
        AkReal32* AK_RESTRICT pBuf = (AkReal32 * AK_RESTRICT)io_pBuffer->GetChannel(i);

        poWriteBuffer->CopyPendingFrameData(pBuf, i, framesPerChannel, PendingFrameTotal);
    }

    PendingFrameTotal += framesPerChannel;

    if (PendingFrameTotal == SAMPLES_PER_FRAME)
    {
        poWriteBuffer->SumFrameData(ChannelCount);
        ApplyWindow_Hann();
        FFT_DiF();
        CalcMag();
        CalcSpectrum();

    #if (FFT_LOGS == 1)
        CopyToPrinters();
    #endif // FFT_LOGS

        res = FrameStatus::Complete;
    }

#if (FFT_LOGS == 1)
    AdvanceSumPrinters(framesPerChannel);
#endif // FFT_LOGS

    return res;
}

void FFT::ApplyWindow_Hann()
{
    SA_Complex* pCur = (SA_Complex*)(poWriteBuffer->poFFT->GetBufferPosition(RequestedChannel::Sum));
    SA_Complex mean = pCur[0] / SA_Complex(SAMPLES_PER_FRAME, 0.0f);

    for (AkUInt32 i = 0; i < SAMPLES_PER_FRAME; ++i)
    {
        pCur[i] -= mean;
        SA_Real multiplier = 0.5 * (1 - cosf(2.0f * (SA_Real)M_PI * i / ((SA_Real)SAMPLES_PER_FRAME - 1.0f)));
        pCur[i] *= multiplier;
    }
}

void FFT::FFT_DiF()
{
    AkUInt16 N = PendingFrameTotal, k, n;
    SA_Real thetaT = M_PI / N;
    SA_Complex phiT(cos(thetaT), -sin(thetaT)), T;

    SA_Complex* pChannelBuff = poWriteBuffer->GetFFTBuffer();

    for (
        AkUInt8 channel = 0; 
        channel < ChannelCount + 1; // + 1 for calculating sum of channels
        ++channel, pChannelBuff += SAMPLES_PER_FRAME
        )
    {
        k = N;
        n = N;
        
        while (k > 1)
        {
            k >>= 1;

            // l increases by a factor of 2 each iteration
            for (AkUInt16 l = 0; l < N; l += n)
            {
                T = 1.0;

                // Butterfly
                for (
                    AkUInt16 a = 0, aIndex = l, bIndex = aIndex + k; 
                    a < k; 
                    ++a, ++aIndex, ++bIndex)
                {
                    SA_Complex t = pChannelBuff[aIndex] - pChannelBuff[bIndex];

                    pChannelBuff[aIndex] += pChannelBuff[bIndex];
                    pChannelBuff[bIndex] = t * T;
 
                    T *= phiT;
                }
            }

            n = k;
        }

        // Decimate
        AkUInt32 m = (AkUInt32)log2(N);
        for (
            AkUInt32 a = 0, b = 0; 
            a < N; 
            ++a, b = a)
        {
            // Reverse bits
            b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
            b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
            b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
            b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
            b = ((b >> 16) | (b << 16)) >> (32 - m);

            if (b > a)
            {
                SA_Complex t = pChannelBuff[a];
                pChannelBuff[a] = pChannelBuff[b];
                pChannelBuff[b] = t;
            }
        }
    }
    
}

void FFT::CalcMag()
{
    SA_Complex* pChannelFFT = poWriteBuffer->GetFFTBuffer();
    SA_Real * pChannelMag = poWriteBuffer->GetMagBuffer();

    for (AkUInt8 channel = 0; 
        channel < ChannelCount + 1; // + 1 for calculating sum of channels
        ++channel, pChannelFFT += SAMPLES_PER_FRAME, pChannelMag += HALF_SAMPLES_PER_FRAME)
    {
        for (AkUInt16 i = 0; i < HALF_SAMPLES_PER_FRAME; ++i)
        {
            pChannelMag[i] = std::abs(pChannelFFT[i]) * NORM_FACTOR;
        }
    }
}

void FFT::CalcSpectrum()
{
    SA_Real* pChannelSpectrum = poWriteBuffer->GetSpectrumBuffer();
    SA_Real* pChannelMag = poWriteBuffer->GetMagBuffer();

    for (
        AkUInt16 channel = 0; 
        channel < ChannelCount + 1;  // + 1 for calculating sum of channels
        ++channel, pChannelSpectrum += FREQ_RESOLUTION, pChannelMag += HALF_SAMPLES_PER_FRAME)
    {
        for (AkUInt16 binIndex = 0; binIndex < FREQ_RESOLUTION; ++binIndex)
        {
            for (AkUInt16 freqIndex = 0; freqIndex < BIN_SIZE; ++freqIndex)
            {
                pChannelSpectrum[binIndex] += pChannelMag[freqIndex];
            }

            pChannelSpectrum[binIndex] /= BIN_SIZE;
        }
    }
}

void FFT::ClearBuffers()
{
    PendingFrameTotal = 0;

    this->poWriteBuffer->SetToZero();
}

// -------------------------------------
// -- END - FFT implementation
// -------------------------------------

// -------------------------------------
// -- FFT::PrintHelper implementation
// -------------------------------------

#if (FFT_LOGS == 1)
// ------------------------------------
// FFT printer methods implementation being
// ------------------------------------
void FFT::InitPrinters()
{
#ifdef FFT_LOGS_SPECTRUM
    SpecPrinterCount = 0;
    SpecPrinters = nullptr;
#endif // FFT_LOGS_SPECTRUM

#ifdef FFT_LOGS_FFT_OUTPUT
    FFTOutPrinterCount = 0;
    FFTOutPrinters = nullptr;
#endif

#ifdef FFT_LOGS_BUFFER_SAMPLES
    SamplePrinterCount = 0;
    SamplePrinters = nullptr;
#endif // FFT_LOGS_BUFFER_SAMPLES
}

void FFT::SetupPrinters(AK::IAkPluginMemAlloc& alloc, AkAudioFormat& in_rFormat)
{
    srand((unsigned int)time(NULL));
    int logID = std::rand();
    time_t t = std::time(nullptr);
    tm* localTime = std::localtime(&t);
    AkUInt16 channelCount = in_rFormat.GetNumChannels();

#ifdef FFT_LOGS_SPECTRUM

    SpecPrinterCount = 1;

    SpecPrinters = AK_PLUGIN_NEW(&alloc, PrintHelper);
    SpecPrinters->Setup(alloc, FREQ_RESOLUTION, localTime, logID, "Spectrum_Sum");

#endif

#ifdef FFT_LOGS_FFT_OUTPUT

    FFTOutPrinterCount = channelCount;

    FFTOutPrinterCount = FFTOutPrinterCount == 1 ? 1 : FFTOutPrinterCount + 1;

    FFTOutPrinters = static_cast<PrintHelper_Complex*>(
        AK_PLUGIN_ALLOC(&alloc, sizeof(PrintHelper_Complex) * FFTOutPrinterCount)
        );

    new (&FFTOutPrinters[FFTOutPrinterCount - 1]) PrintHelper_Complex();
    FFTOutPrinters[FFTOutPrinterCount - 1].Setup(alloc, SAMPLES_PER_FRAME, localTime, logID, "FFTOutput_Sum");

    for (AkUInt16 i = 0; i < FFTOutPrinterCount - 1; ++i)
    {
        std::string fileString("FFTOutput_Channel" + std::to_string(i));
        new (&FFTOutPrinters[i]) PrintHelper_Complex();
        FFTOutPrinters[i].Setup(alloc, SAMPLES_PER_FRAME, localTime, logID, fileString.c_str());
    }
#endif

#ifdef FFT_LOGS_BUFFER_SAMPLES

    SamplePrinterCount = channelCount;

    SamplePrinterCount = SamplePrinterCount == 1 ? 1 : SamplePrinterCount + 1;

    SamplePrinters = static_cast<PrintHelper*>(
        AK_PLUGIN_ALLOC(&alloc, sizeof(PrintHelper) * SamplePrinterCount)
        );

    new (&SamplePrinters[SamplePrinterCount - 1]) PrintHelper();
    SamplePrinters[SamplePrinterCount - 1].Setup(alloc, SAMPLES_PER_FRAME, localTime, logID, "ChannelSum");

    for (AkUInt16 i = 0; i < SamplePrinterCount - 1; ++i)
    {
        std::string fileString("Channel" + std::to_string(i));

        new (&SamplePrinters[i]) PrintHelper();
        SamplePrinters[i].Setup(alloc, SAMPLES_PER_FRAME, localTime, logID, fileString.c_str());
    }
#endif
}

void FFT::PreExecutePrinters(AkUInt16 numChannels) const
{
#ifdef FFT_LOGS_FFT_OUTPUT
    if (numChannels > FFTOutPrinterCount - 1)
    {
        throw(std::runtime_error("Unhandled printing logic for change of number of channels"));
    }
#endif // FFT_LOGS_SPECTRUM

#ifdef FFT_LOGS_BUFFER_SAMPLES
    if (numChannels > SamplePrinterCount - 1)
    {
        throw(std::runtime_error("Unhandled printing logic for change of number of channels"));
    }
#endif // FFT_LOGS_BUFFER_SAMPLES

}

void FFT::AdvanceSumPrinters(AkUInt16 elements)
{
    #ifdef FFT_LOGS_FFT_OUTPUT

        if(FFTOutPrinterCount > 1) FFTOutPrinters[FFTOutPrinterCount - 1].AdvanceLocPointer(elements);

    #endif // FFT_LOGS_BUFFER_SAMPLES

    #ifdef FFT_LOGS_BUFFER_SAMPLES

        if (SamplePrinterCount > 1) SamplePrinters[SamplePrinterCount - 1].AdvanceLocPointer(elements);

    #endif // FFT_LOGS_BUFFER_SAMPLES
}

void FFT::DestroyAndWritePrinters(AK::IAkPluginMemAlloc* pAlloc)
{
#ifdef FFT_LOGS_SPECTRUM

    for (AkUInt16 i = 0; i < SpecPrinterCount; ++i)
    {
        SpecPrinters[i].WriteAndCleanup(*pAlloc);
    }

    if (SpecPrinterCount == 1) AK_PLUGIN_DELETE(pAlloc, SpecPrinters);
    else  AK_PLUGIN_FREE(pAlloc, SpecPrinters);

#endif // #ifdef FFT_LOGS_SPECTRUM

#ifdef FFT_LOGS_FFT_OUTPUT

    for (AkUInt16 i = 0; i < FFTOutPrinterCount; ++i)
    {
        FFTOutPrinters[i].WriteAndCleanup(*pAlloc);
    }

    AK_PLUGIN_FREE(pAlloc, FFTOutPrinters);

#endif // FFT_LOGS_FFT_OUTPUT


#ifdef FFT_LOGS_BUFFER_SAMPLES

    for (AkUInt16 i = 0; i < SamplePrinterCount; ++i)
    {
        SamplePrinters[i].WriteAndCleanup(*pAlloc);
    }

    AK_PLUGIN_FREE(pAlloc, SamplePrinters);

#endif // FFT_LOGS_BUFFER_SAMPLES

}

void FFT::CopyToPrinters()
{
#ifdef FFT_LOGS_FFT_OUTPUT
    CopyAndSumPrinterData(FFTOutPrinters, SAMPLES_PER_FRAME, poFFTWriteBuffers);
#endif // FFT_LOGS_FFT_OUTPUT

    // Copy data to spectrum loggers
#ifdef FFT_LOGS_SPECTRUM
   SpecPrinters->Copy(FREQ_RESOLUTION, poSpectrumBuffer);
#endif
}

void FFT::CopyAndSumPrinterData(PrintHelper* printersRoot, AkUInt16 elementsPerBuffer, std::vector<SA_Real*>& bufferVec)
{
    if (bufferVec.size() > 1)
    {
        SA_Real* pSumBuff = bufferVec.back();
        PrintHelper* pSumPrint = &printersRoot[bufferVec.size() - 1];

        for (AkUInt32 i = 0; i < bufferVec.size(); ++i)
        {
            printersRoot[i].Copy(elementsPerBuffer, bufferVec[i]);
            pSumPrint->Sum(elementsPerBuffer, bufferVec[i]);
        }
    }
    else
    {
        printersRoot->Copy(elementsPerBuffer, bufferVec.front());
    }
}

void FFT::CopyAndSumPrinterData(PrintHelper_Complex* printersRoot, AkUInt16 elementsPerBuffer, std::vector<SA_Complex*> bufferVec)
{
    if (FFTOutPrinterCount > 1)
    {
        PrintHelper_Complex* pSumPrint = &printersRoot[FFTOutPrinterCount - 1];

        for (AkUInt32 i = 0; i < FFTOutPrinterCount; ++i)
        {
            printersRoot[i].Copy(elementsPerBuffer, bufferVec[i]);
            pSumPrint->Sum(elementsPerBuffer, bufferVec[i]);
        }
    }
    else
    {
        printersRoot->Copy(elementsPerBuffer, bufferVec.front());
    }
}

// ------------------------------------
// FFT printer methods implementation end
// ------------------------------------
// ------------------------------------
// FFT::PrintHelper implementation begin
// ------------------------------------

void  FFT::PrintHelper::Setup(AK::IAkPluginMemAlloc& alloc, AkUInt32 stride, tm* localTime, int logID, const char* logName)
{
    Stride = stride;

    AkUInt32 count = stride * MAX_ITEM_COUNT;
    AkUInt32 size = count * sizeof(SA_Real);
    poPrintBuffer = static_cast<SA_Real*>(AK_PLUGIN_ALLOC(&alloc, size));
    memset(poPrintBuffer, 0x0, size);

    pLoc = poPrintBuffer;
    pEnd = poPrintBuffer + count;

    std::ostringstream frameFile;
    frameFile << "log/" << std::put_time(localTime, "%H-%M-%S") << "_" << logName << "_" << logID << ".txt";
    std::string file = frameFile.str();
    poLogFile = fopen(file.c_str(), "w");
    if (!poLogFile) { throw(std::runtime_error("log file creation failure")); }
}

void FFT::PrintHelper::WriteAndCleanup(AK::IAkPluginMemAlloc& alloc)
{
    WriteToFile();

    fclose(poLogFile);

    FreeBuffers(alloc);
}

bool FFT::PrintHelper::Copy(AkUInt16 frameCount, SA_Real* buffer)
{
    if (IsFull()) return false;
    if (pLoc + frameCount > pEnd)
    {
        memset(pLoc, 0x0, pEnd - pLoc);
        pLoc = pEnd;

        return false;
    }
    memcpy_s(pLoc, (pEnd - poPrintBuffer) * sizeof(SA_Real), buffer, frameCount * sizeof(SA_Real));

    pLoc += frameCount;

    return true;
}

bool FFT::PrintHelper::Sum(AkUInt16 frameCount, SA_Real* buffer)
{
    if (IsFull()) return false;
    if (pLoc + frameCount > pEnd)
    {
        memset(pLoc, 0x0, pEnd - pLoc);
        pLoc = pEnd;

        return false;
    }

    SA_Real* pCur = pLoc;
    for (AkUInt16 j = 0; j < frameCount; ++j)
    {
        (pCur[j]) += (buffer[j]);
    }

    return true;
}

inline void FFT::PrintHelper::AdvanceLocPointer(AkUInt16 frameCount)
{
    if (!IsFull() && pLoc + frameCount <= pEnd)
    {
        pLoc += frameCount;
    }
    else
    {
        pLoc = pEnd;
    }
}

void FFT::PrintHelper::WriteToFile()
{
    AkUInt32 size = pLoc - poPrintBuffer;
    AkUInt32 frameCount = (size + Stride - 1) / Stride;

    for (AkUInt32 i = 0; i < frameCount; ++i)
    {
        for (AkUInt16 j = 0; j < Stride; ++j)
        {
            fprintf(poLogFile, "%f ", poPrintBuffer[(i * Stride) + j]);
        }
        fprintf(poLogFile, "\n");
    }
}

inline void FFT::PrintHelper::FreeBuffers(AK::IAkPluginMemAlloc& alloc)
{
    AK_PLUGIN_FREE(&alloc, poPrintBuffer);
}

// ------------------------------------
// FFT::PrintHelper implementation end
// ------------------------------------
// ------------------------------------
// FFT::PrintHelper_Comples implementation ends
// ------------------------------------

void FFT::PrintHelper_Complex::Setup(AK::IAkPluginMemAlloc& alloc, AkUInt32 stride, tm* localTime, int logID, const char* logName)
{
    Stride = stride;

    AkUInt32 count = stride * MAX_ITEM_COUNT;
    AkUInt32 size = count * sizeof(SA_Complex);
    poComplexPrint = static_cast<SA_Complex*>(AK_PLUGIN_ALLOC(&alloc, size));
    memset(poComplexPrint, 0x0, size);

    pComplexLoc = poComplexPrint;
    pComplexEnd = poComplexPrint + count;

    std::ostringstream frameFile;
    frameFile << "log/" << std::put_time(localTime, "%H-%M-%S") << "_" << logName << "_" << logID << ".txt";
    std::string file = frameFile.str();
    poLogFile = fopen(file.c_str(), "w");
    if (!poLogFile) { throw(std::runtime_error("log file creation failure")); }
}

bool FFT::PrintHelper_Complex::Copy(AkUInt16 frameCount, SA_Complex* buffer)
{
    if (IsFull()) return false;
    if (pComplexLoc + frameCount > pComplexEnd)
    {
        memset(pComplexLoc, 0x0, pComplexEnd - pComplexLoc);
        pComplexLoc = pComplexEnd;

        return false;
    }
    memcpy_s(pComplexLoc, (pComplexEnd - poComplexPrint) * sizeof(SA_Complex), buffer, frameCount * sizeof(SA_Complex));

    pComplexLoc += frameCount;

    return true;
}

bool FFT::PrintHelper_Complex::Sum(AkUInt16 frameCount, SA_Complex* buffer)
{
    if (IsFull()) return false;
    if (pComplexLoc + frameCount > pComplexEnd)
    {
        memset(pComplexLoc, 0x0, pComplexEnd - pComplexLoc);
        pComplexLoc = pComplexEnd;

        return false;
    }

    SA_Complex* pCur = pComplexLoc;
    for (AkUInt16 j = 0; j < frameCount; ++j)
    {
        (pCur[j]) += (buffer[j]);
    }

    return true;
}

void FFT::PrintHelper_Complex::WriteToFile()
{
    AkUInt32 size = pComplexLoc - poComplexPrint;
    AkUInt32 frameCount = (size + Stride - 1) / Stride;

    for (AkUInt32 i = 0; i < frameCount; ++i)
    {
        for (AkUInt16 j = 0; j < Stride; ++j)
        {
            fprintf(poLogFile, "%f+%f ", poComplexPrint[(i * Stride) + j].real(), poComplexPrint[(i * Stride) + j].imag());
        }


        fprintf(poLogFile, "\n");
    }
}

void FFT::PrintHelper_Complex::FreeBuffers(AK::IAkPluginMemAlloc& alloc)
{
    AK_PLUGIN_FREE(&alloc, poComplexPrint);
}

// ------------------------------------
// FFT::PrintHelper_Complex implementation ends
// ------------------------------------


#endif // FFT_LOGS
