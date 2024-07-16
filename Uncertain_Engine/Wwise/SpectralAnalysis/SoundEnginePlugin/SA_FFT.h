#pragma once
#include "_def_SA.h"

#include < AK/SoundEngine/Common/AkCommonDefs.h >

#include <complex>
#include <valarray>
#include <mutex>
#include <vector>
#include <ctime>

#include "SpectralDAO.h"

#ifndef M_PI
#define M_PI 3.141592653f
#endif

// TODO: Fix logs - broken since CL235506
#define FFT_LOGS 0
#if (FFT_LOGS == 1)

#define FFT_LOGS_BUFFER_SAMPLES
#define FFT_LOGS_FFT_OUTPUT
#define FFT_LOGS_SPECTRUM

#endif

class FFT
{
public:

	static const AkUInt16 SAMPLES_PER_FRAME =	    1024;
	static const AkUInt16 HALF_SAMPLES_PER_FRAME =	SAMPLES_PER_FRAME / 2;

	static		 AkReal32 NORM_FACTOR;
	static const AkUInt32 SAMPLE_FREQUENCY =		48000; 

	static const AkUInt32 FREQ_RESOLUTION =			((SAMPLE_FREQUENCY / 2) + (SAMPLES_PER_FRAME - 1)) / SAMPLES_PER_FRAME;
	static const AkUInt32 BIN_SIZE =				HALF_SAMPLES_PER_FRAME / FREQ_RESOLUTION;
	static		 AkReal32 FFT_VAL_SPECTRUM_WEIGHT;

	static const AkUInt32 SPECTRUM_BUFFER_SAMPLES = FREQ_RESOLUTION;
	static const AkUInt32 MAG_BUFFER_SAMPLES =		HALF_SAMPLES_PER_FRAME;
	static const AkUInt32 FRAME_BUFFER_SAMPLES =	SAMPLES_PER_FRAME;

	static const AkUInt32 REAL_SAMPLE_SIZE =		sizeof(AkReal32);
	static const AkUInt32 COMPLEX_SAMPLE_SIZE =		sizeof(std::complex<AkReal32>);

	static const AkUInt32 FRAME_SAMPLE_BIT_SIZE =	COMPLEX_SAMPLE_SIZE;
	static const AkUInt32 MAG_SAMPLE_BIT_SIZE =		REAL_SAMPLE_SIZE;
	static const AkUInt32 SPECTRUM_SAMPLE_BIT_SIZE =REAL_SAMPLE_SIZE;

	static const AkUInt32 FRAME_BUFFER_BITS =		SAMPLES_PER_FRAME * FRAME_SAMPLE_BIT_SIZE;
	static const AkUInt32 MAG_BUFFER_BITS =			MAG_BUFFER_SAMPLES * MAG_SAMPLE_BIT_SIZE;
	static const AkUInt32 SPECTRUM_BUFFER_BITS =	FREQ_RESOLUTION * SPECTRUM_SAMPLE_BIT_SIZE;


	enum class FrameStatus : AkUInt8
	{
		Incomplete  = 0,
		Complete	= 1
	};

	FFT();
	~FFT() = default;

	void Setup(AK::IAkPluginMemAlloc* in_pAllocator, AkAudioFormat& in_rFormat);
	void Destroy(AK::IAkPluginMemAlloc* in_pAllocator);

	FrameStatus Execute(AkAudioBuffer* io_pBuffer);
	void ClearBuffers();

private:
	void CopyChannelBuffer(AkReal32* io_pBuffer, AkUInt16 index, AkUInt16 count);

	void ApplyWindow_Hann();
	void FFT_DiF();
	void CalcMag();
	void CalcSpectrum();

	friend class SpectralAnalysisFX;
	SpectralDAO* GetWriteBuffer() { return poWriteBuffer; }

	SpectralDAO* poWriteBuffer;

	AkUInt32 SampleRate;
	AkUInt16 PendingFrameTotal;
	AkUInt8 ChannelCount;

	const char _pad1[5] = { 0 };


	/// Logs are broken
#if (FFT_LOGS == 1)
protected:
	struct PrintHelper
	{
	#define PRINT_HELPER_OVERFLOW_CHECK
		const static AkUInt16 MAX_ITEM_COUNT = 1000;
	public:
		PrintHelper() = default;
		PrintHelper(const PrintHelper&) = delete;
		PrintHelper& operator=(const PrintHelper&) = delete;
		~PrintHelper() = default;

		virtual void Setup(AK::IAkPluginMemAlloc& alloc, AkUInt32 stride, tm* localTime, int logID, const char* logName);

		virtual void WriteAndCleanup(AK::IAkPluginMemAlloc& alloc);

		PrintHelper& operator=(const 
			
			& num) { *pLoc++ = num; return *this; }
		void Sum(const AkUInt32& index, const SA_Real& num) { (pLoc[index]) += num; }

		bool Copy(AkUInt16 frameCount, SA_Real* buffer);
		bool Sum(AkUInt16 frameCount, SA_Real* buffer);

		virtual void AdvanceLocPointer(AkUInt16 frameCount);

		virtual AkUInt32 TotalSize() const { return sizeof(SA_Real) * Stride; }
		virtual bool IsFull() { return pLoc >= pEnd; }

	protected:
		virtual void WriteToFile();
		virtual void FreeBuffers(AK::IAkPluginMemAlloc& alloc);

		SA_Real* poPrintBuffer;
		SA_Real* pLoc;
		SA_Real* pEnd;
		AkUInt32 Stride;
		FILE* poLogFile;
		char pad[8] = { 0 };
	};

	struct PrintHelper_Complex : public PrintHelper
	{
		virtual void Setup(AK::IAkPluginMemAlloc& alloc, AkUInt32 frameCountEstimate, tm* localTime, int logID, const char* logName);

		bool Copy(AkUInt16 frameCount, SA_Complex* buffer);
		bool Sum(AkUInt16 frameCount, SA_Complex* buffer);

		virtual AkUInt32 TotalSize() const { return sizeof(SA_Complex) * Stride; }
		virtual bool IsFull() override { return pComplexLoc >= pComplexEnd; }

	protected:
		virtual void WriteToFile() override;
		virtual void FreeBuffers(AK::IAkPluginMemAlloc& alloc);

		SA_Complex* poComplexPrint;
		SA_Complex* pComplexLoc;
		SA_Complex* pComplexEnd;
		char pad[4] = { 0 };
	};

	void InitPrinters();
	void SetupPrinters(AK::IAkPluginMemAlloc& alloc, AkAudioFormat& in_rFormat);
	void PreExecutePrinters(AkUInt16 numChannels) const;
	void AdvanceSumPrinters(AkUInt16 elements);
	void CopyToPrinters();
	void DestroyAndWritePrinters(AK::IAkPluginMemAlloc* pAlloc);

	void CopyAndSumPrinterData(PrintHelper* printersRoot, AkUInt16 elementsPerBuffer, std::vector<SA_Real*>& bufferVec);
	void CopyAndSumPrinterData(PrintHelper_Complex* printersRoot, AkUInt16 elementsPerBuffer, std::vector<SA_Complex*> bufferVec);


#ifdef FFT_LOGS_SPECTRUM

	AkUInt16 SpecPrinterCount;
	PrintHelper* SpecPrinters;

#endif 

#ifdef FFT_LOGS_FFT_OUTPUT

	AkUInt16 FFTOutPrinterCount;
	PrintHelper_Complex* FFTOutPrinters;

#endif

#ifdef FFT_LOGS_BUFFER_SAMPLES

	AkUInt16 SamplePrinterCount;
	PrintHelper* SamplePrinters;

#endif 
#endif // FFT_LOGS

};