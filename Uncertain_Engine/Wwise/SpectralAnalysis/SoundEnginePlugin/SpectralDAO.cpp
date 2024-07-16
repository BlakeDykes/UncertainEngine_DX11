#include "SpectralDAO.h"
#include "SpectralRequest.h"
#include "SA_FFT.h"
#include <mutex>


SpectralDAO::SpectralDAO()
    : mtx()
    , DataToPull(RequestedData::PushOnly)
    , ChannelsToPull(RequestedChannel::All)
    , poFFT(nullptr)
    , poMag(nullptr)
    , poSpectrum(nullptr)
    , bNewDataSinceLastReq(false)
{
}

SpectralDAO::SpectralDAO(RequestedData dataToPull, RequestedChannel channelToPull)
    : mtx()
    , DataToPull(dataToPull)
    , ChannelsToPull(channelToPull)
    , poFFT(nullptr)
    , poMag(nullptr)
    , poSpectrum(nullptr)
{
}

void SpectralDAO::Init_AkAlloc(AK::IAkPluginMemAlloc* pAlloc, RequestedData dataToPull, RequestedChannel channelsToPull) 
{
    std::lock_guard<std::timed_mutex> lg(mtx);

    DataToPull = dataToPull;
    ChannelsToPull = channelsToPull;

    // FFT buffer
    if (SAHelpers::ReqFFT(dataToPull))
    {
        this->poFFT = AK_PLUGIN_NEW(pAlloc, SpectralBuffer_Complex);
        this->poFFT->Setup(*pAlloc, (AkUInt32)FFT::SAMPLES_PER_FRAME, channelsToPull);
    }
    else
    {
        this->poFFT = AK_PLUGIN_NEW(pAlloc, SpectralBuffer);
    }

    // Magnitude buffer
    if (SAHelpers::ReqMagnitude(dataToPull))
    {
        this->poMag = AK_PLUGIN_NEW(pAlloc, SpectralBuffer_Real);
        this->poMag->Setup(*pAlloc, FFT::HALF_SAMPLES_PER_FRAME, channelsToPull);
    }
    else
    {
        this->poMag = AK_PLUGIN_NEW(pAlloc, SpectralBuffer);
    }

    // Spectrum buffer
    if (SAHelpers::ReqSpectrum(dataToPull))
    {
        this->poSpectrum = AK_PLUGIN_NEW(pAlloc, SpectralBuffer_Real);
        this->poSpectrum->Setup(*pAlloc, FFT::FREQ_RESOLUTION, channelsToPull);
    }
    else
    {
        this->poSpectrum = AK_PLUGIN_NEW(pAlloc, SpectralBuffer);
    }
}

void SpectralDAO::Destroy_AkAlloc(AK::IAkPluginMemAlloc* pAlloc)
{
    std::lock_guard<std::timed_mutex> lg(mtx);

    poFFT->Destroy(pAlloc);
    AK_PLUGIN_DELETE(pAlloc, poFFT);

    poMag->Destroy(pAlloc);
    AK_PLUGIN_DELETE(pAlloc, poMag);

    poSpectrum->Destroy(pAlloc);
    AK_PLUGIN_DELETE(pAlloc, poSpectrum);
}

void SpectralDAO::Init_GameAlloc(RequestedData dataToPull, RequestedChannel channelsToPull)
{
    std::lock_guard<std::timed_mutex> lg(mtx);

    DataToPull = dataToPull;
    ChannelsToPull = channelsToPull;

    if (SAHelpers::ReqFFT(DataToPull))
    {
        this->poFFT = new SpectralBuffer_Complex();
        this->poFFT->Setup((AkUInt32)FFT::SAMPLES_PER_FRAME, channelsToPull);
    }
    else
    {
        this->poFFT = new SpectralBuffer();
    }

    if (SAHelpers::ReqMagnitude(DataToPull))
    {
        this->poMag = new SpectralBuffer_Real();
        this->poMag->Setup(FFT::HALF_SAMPLES_PER_FRAME, channelsToPull);
    }
    else
    {
        this->poMag = new SpectralBuffer();
    }

    if (SAHelpers::ReqSpectrum(DataToPull))
    {
        this->poSpectrum = new SpectralBuffer_Real();
        this->poSpectrum->Setup(FFT::FREQ_RESOLUTION, channelsToPull);
    }
    else
    {
        this->poSpectrum = new SpectralBuffer();
    }
}

void SpectralDAO::Destroy_GameAlloc()
{
    std::lock_guard<std::timed_mutex> lg(mtx);

    poFFT->Destroy();
    delete poFFT;

    poMag->Destroy();
    delete poMag;

    poSpectrum->Destroy();
    delete poSpectrum;
}

void SpectralDAO::CopyPendingFrameData(
    AkReal32* frameData
    , AkUInt16 channelIndex
    , AkUInt32 frameCount
    , AkUInt32 offset
)
{
    this->poFFT->CopyPendingFrameData(frameData, channelIndex, frameCount, offset);
}

void SpectralDAO::SumFrameData(AkUInt8 channelCount)
{
    this->poFFT->SumChannels(channelCount);
}

void SpectralDAO::Pull(SpectralDAO* pIn)
{
    if (mtx.try_lock_for(LOCK_WAIT_DURATION))
    {
        this->poFFT->Pull(pIn->poFFT);
        this->poMag->Pull(pIn->poMag);
        this->poSpectrum->Pull(pIn->poSpectrum);

        bNewDataSinceLastReq = true;
        mtx.unlock();
    }
}

void SpectralDAO::ProcessRequests(std::list<SpectralRequest*>::iterator beginPos, std::list<SpectralRequest*>::iterator endPos)
{
    if (bNewDataSinceLastReq)
    {
    
        std::lock_guard<std::timed_mutex> lg(mtx);

        SpectralRequest::DataBundle data(
            static_cast<SpectralBuffer_Complex&>(*this->poFFT)
            , static_cast<SpectralBuffer_Real&>(*this->poMag)
            , static_cast<SpectralBuffer_Real&>(*this->poSpectrum)
        );
    
        while (beginPos != endPos)
        {
            (*beginPos)->Execute(data);
            ++beginPos;
        }

        bNewDataSinceLastReq = false;
    }
}

void SpectralDAO::SetToZero()
{
    this->poFFT->Clear();
    this->poMag->Clear();
    this->poSpectrum->Clear();
}

SpectralBuffer_Complex* SpectralDAO::GetFFTSB()
{
    return static_cast<SpectralBuffer_Complex*>(poFFT);
}

SpectralBuffer_Real* SpectralDAO::GetMagSB()
{
    return static_cast<SpectralBuffer_Real*>(poMag);
}

SpectralBuffer_Real* SpectralDAO::GetSpectrumSB()
{
    return static_cast<SpectralBuffer_Real*>(poSpectrum);
}

SA_Complex* SpectralDAO::GetFFTBuffer(AkUInt32 offset)
{
    return static_cast<SA_Complex*>(poFFT->GetBufferPosition(offset));
}


SA_Real* SpectralDAO::GetMagBuffer(AkUInt32 offset)
{
    return static_cast<SA_Real*>(poMag->GetBufferPosition(offset));
}

SA_Real* SpectralDAO::GetSpectrumBuffer(AkUInt32 offset)
{
    return static_cast<SA_Real*>(poSpectrum->GetBufferPosition(offset));
}


