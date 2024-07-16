#pragma once
#include "_def_SA.h"

#include <AK/SoundEngine/Common/AkCommonDefs.h>

enum class RequestedData : AkUInt8
{
	PushOnly = 0,
	Spectrum = 1,
	Magnitude = 2,
	Spectrum_Magnitude = 3,
	FFT = 4,
	Spectrum_FFT = 5,
	Magnitude_FFT = 6,
	Spectrum_Magnitude_FFT = 7,

	All = 0xFF
};

enum class RequestedChannel : AkInt8
{
	Sum = 0,
	Channel_1 = 1,
	Channel_2 = 2,
	Channel_1_2 = 3,

	All = 0x7F
};

struct SAHelpers
{
	static AkInt8 GetChannelCount(RequestedChannel req);
	static AkInt8 GetDataCount(RequestedData req);
	static AkInt32 GetPullOffset(AkUInt32 stride, RequestedChannel reqChannel);

	static bool IsPushOnly(RequestedData& req) { return (((AkUInt8)req & (AkUInt8)RequestedData::PushOnly) > 0); }
	static bool ReqSpectrum(RequestedData& req) { return req == RequestedData::PushOnly || (((AkUInt8)req & (AkUInt8)RequestedData::Spectrum) > 0); }
	static bool ReqMagnitude(RequestedData& req) { return req == RequestedData::PushOnly || (((AkUInt8)req & (AkUInt8)RequestedData::Magnitude) > 0); }
	static bool ReqFFT(RequestedData& req) { return req == RequestedData::PushOnly || (((AkUInt8)req & (AkUInt8)RequestedData::FFT) > 0); }
	static bool ReqDataAll(RequestedData& req) { return req == RequestedData::PushOnly || req == RequestedData::All; }

	static bool ReqChannel1(RequestedChannel& req) { return (((AkUInt8)req & (AkUInt8)RequestedChannel::Channel_1) > 0); }
	static bool ReqChannel2(RequestedChannel& req) { return (((AkUInt8)req & (AkUInt8)RequestedChannel::Channel_2) > 0); }
	static bool ReqChannelSum(RequestedChannel& req) { return (((AkUInt8)req & (AkUInt8)RequestedChannel::Sum) > 0); }
	static bool ReqChannelAll(RequestedChannel& req) { return req == RequestedChannel::All; }
};

struct SpectralBuffer
{
	virtual void CopyPendingFrameData(
		AkReal32* frameData
		, AkUInt16 channelIndex
		, AkUInt32 frameCount
		, AkUInt32 offset
	);

	virtual void SumChannels(AkUInt8 channelCount);

	virtual void Pull(const SpectralBuffer* dataIn);

	virtual void* GetBufferPosition(const AkUInt32& pullOffset) const;
	virtual void* GetBufferPosition(RequestedChannel reqChannel) const;

	virtual void Setup(AK::IAkPluginMemAlloc& alloc, const AkUInt32& stride, RequestedChannel reqChannel);
	virtual void Setup(const AkUInt32& stride, RequestedChannel reqChannel);

	virtual void Destroy(AK::IAkPluginMemAlloc* alloc);
	virtual void Destroy();

	virtual void Clear();

	AkUInt32 GetChannelBufferSize() const { return ChannelBufferSize; }
	AkUInt32 GetTotalBufferSize() const { return TotalBufferSize; }

protected:
	AkUInt32 PullOffset;
	AkUInt32 ChannelBufferSize;
	AkUInt32 TotalBufferSize;
};

struct SpectralBuffer_Real : public SpectralBuffer
{
	virtual void SumChannels(AkUInt8 channelCount) override;

	virtual void Pull(const SpectralBuffer* dataIn) override;

	virtual void* GetBufferPosition(const AkUInt32& pullOffset) const override;
	virtual void* GetBufferPosition(RequestedChannel reqChannel) const;

	virtual void Setup(AK::IAkPluginMemAlloc& alloc, const AkUInt32& stride, RequestedChannel reqChannel) override;
	virtual void Setup(const AkUInt32& stride, RequestedChannel reqChannel) override;

	virtual void Destroy(AK::IAkPluginMemAlloc* alloc) override;
	virtual void Destroy() override;

	virtual void Clear() override;

private:
	SA_Real* poBuff;
	AkUInt32  Stride;
};

struct SpectralBuffer_Complex : public SpectralBuffer
{
	virtual void CopyPendingFrameData(
		AkReal32* frameData
		, AkUInt16 channelIndex
		, AkUInt32 frameCount
		, AkUInt32 offset
	);

	virtual void SumChannels(AkUInt8 channelCount) override;
	
	virtual void Pull(const SpectralBuffer* dataIn) override;

	void* GetBufferPosition(const AkUInt32& pullOffset) const override;
	virtual void* GetBufferPosition(RequestedChannel reqChannel) const;

	virtual void Setup(AK::IAkPluginMemAlloc& alloc, const AkUInt32& stride, RequestedChannel reqChannel) override;
	virtual void Setup(const AkUInt32& stride, RequestedChannel reqChannel) override;
	
	virtual void Destroy(AK::IAkPluginMemAlloc* alloc) override;
	virtual void Destroy() override;

	virtual void Clear() override;

private:
	SA_Complex* poBuff;
	AkUInt32  Stride;
};
