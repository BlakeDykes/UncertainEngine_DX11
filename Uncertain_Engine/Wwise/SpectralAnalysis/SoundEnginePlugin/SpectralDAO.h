#pragma once

#include "SpectralBuffer.h"
#include <mutex>
#include <list>

class SpectralRequest;

static const std::chrono::microseconds LOCK_WAIT_DURATION(6);
static const AkGameObjectID PUSH_ONLY_OBJECT = 0;

struct SpectralDAO
{
public:
	SpectralDAO();
	SpectralDAO(const SpectralDAO&) = default;
	SpectralDAO& operator=(const SpectralDAO&) = default;
	~SpectralDAO() = default;

	SpectralDAO(RequestedData dataToPull, RequestedChannel channelToPull);

	void Init_AkAlloc(
		AK::IAkPluginMemAlloc* pAlloc
		, RequestedData dataToPull = RequestedData::PushOnly
		, RequestedChannel channelsToPull = RequestedChannel::All
	);
	void Destroy_AkAlloc(AK::IAkPluginMemAlloc* pAlloc);

	void Init_GameAlloc(
		RequestedData dataToPull = RequestedData::PushOnly
		, RequestedChannel channelsToPull = RequestedChannel::All
	);

	void Destroy_GameAlloc();

	void CopyPendingFrameData(
		AkReal32* frameData
		, AkUInt16 channelIndex
		, AkUInt32 frameCount
		, AkUInt32 offset
	);

	void SumFrameData(AkUInt8 channelCount);

	void Pull(SpectralDAO* pIn);

	void ProcessRequests(std::list<SpectralRequest*>::iterator beginPos, std::list<SpectralRequest*>::iterator endPos);

	void SetToZero();

	RequestedData GetDataReq() const { return this->DataToPull; }
	RequestedChannel GetChannelReq() const { return this->ChannelsToPull; }

protected:
	std::timed_mutex mtx;

	RequestedData DataToPull;
	RequestedChannel ChannelsToPull;
	const char _pad1[2] = { 0 };

	SpectralBuffer*			poFFT;
	SpectralBuffer*			poMag;
	SpectralBuffer*			poSpectrum;
	bool					bNewDataSinceLastReq;

private:
	friend class FFT;
	SpectralBuffer_Complex* GetFFTSB();
	SpectralBuffer_Real*	GetMagSB();
	SpectralBuffer_Real*	GetSpectrumSB();

	SA_Complex* GetFFTBuffer(AkUInt32 offset = 0);
	SA_Real*				GetMagBuffer(AkUInt32 offset = 0);
	SA_Real*				GetSpectrumBuffer(AkUInt32 offset = 0);

};

