#include "SpectralBuffer.h"
#include "SA_FFT.h"

void SpectralBuffer::CopyPendingFrameData(AkReal32* frameData, AkUInt16 channelIndex, AkUInt32 frameCount, AkUInt32 offset)
{
	AK_UNUSEDVAR(frameData);
	AK_UNUSEDVAR(channelIndex);
	AK_UNUSEDVAR(frameCount);
}

void SpectralBuffer::SumChannels(AkUInt8 channelCount)
{
	AK_UNUSEDVAR(channelCount);
}

void SpectralBuffer::Pull(const SpectralBuffer* dataIn)
{
	AK_UNUSEDVAR(dataIn);
}

void* SpectralBuffer::GetBufferPosition(const AkUInt32& pullOffset) const
{
	AK_UNUSEDVAR(pullOffset);
	return nullptr;
}

void* SpectralBuffer::GetBufferPosition(RequestedChannel reqChannel) const
{
	AK_UNUSEDVAR(reqChannel);
	return nullptr;
}

void SpectralBuffer::Setup(AK::IAkPluginMemAlloc& alloc, const AkUInt32& stride, RequestedChannel reqChannel)
{
	AK_UNUSEDVAR(alloc);
	AK_UNUSEDVAR(stride);
	AK_UNUSEDVAR(reqChannel);
}

void SpectralBuffer::Setup(const AkUInt32& stride, RequestedChannel reqChannel)
{
	AK_UNUSEDVAR(stride);
	AK_UNUSEDVAR(reqChannel);
}

void SpectralBuffer::Destroy(AK::IAkPluginMemAlloc* alloc)
{
	AK_UNUSEDVAR(alloc);
	AK_UNUSEDVAR(alloc);
}

void SpectralBuffer::Destroy()
{
}

void SpectralBuffer::Clear()
{
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// SpectralBuffer_Real implementation begins----------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

void SpectralBuffer_Real::SumChannels(AkUInt8 channelCount)
{
	SA_Real* pSumBuffer = this->poBuff + (Stride * channelCount);

	memcpy_s(pSumBuffer, ChannelBufferSize, poBuff, ChannelBufferSize);

	for (AkUInt32 i = 1; i < channelCount; ++i)
	{
		SA_Real* pCurChannel = poBuff + (i * Stride);

		for (AkUInt32 j = 0; j < Stride; ++j)
		{
			pSumBuffer[j] += pCurChannel[j];
		}
	}
}

void SpectralBuffer_Real::Pull(const SpectralBuffer* dataIn)
{
	const SpectralBuffer_Real* pIn = static_cast<const SpectralBuffer_Real*>(dataIn);

	memcpy_s(this->poBuff, TotalBufferSize, pIn->GetBufferPosition(PullOffset), TotalBufferSize);
}

void* SpectralBuffer_Real::GetBufferPosition(const AkUInt32& pullOffset) const
{
	return (void*)(poBuff + pullOffset);
}

void* SpectralBuffer_Real::GetBufferPosition(RequestedChannel reqChannel) const
{
	return (void*)(poBuff + SAHelpers::GetPullOffset(Stride, reqChannel));
}

void SpectralBuffer_Real::Setup(AK::IAkPluginMemAlloc& alloc, const AkUInt32& stride, RequestedChannel reqChannel)
{
	Stride = stride;
	PullOffset = SAHelpers::GetPullOffset(stride, reqChannel);
	ChannelBufferSize = stride * FFT::REAL_SAMPLE_SIZE;
	TotalBufferSize = SAHelpers::GetChannelCount(reqChannel) * ChannelBufferSize;

	poBuff = static_cast<SA_Real*>(AK_PLUGIN_ALLOC(&alloc, TotalBufferSize, 16));
	memset(poBuff, 0x0, TotalBufferSize);
}

void SpectralBuffer_Real::Setup(const AkUInt32& stride, RequestedChannel reqChannel)
{
	Stride = stride;
	PullOffset = SAHelpers::GetPullOffset(stride, reqChannel);
	ChannelBufferSize = stride * FFT::REAL_SAMPLE_SIZE;
	AkUInt32 count = SAHelpers::GetChannelCount(reqChannel);
	TotalBufferSize = count * ChannelBufferSize;

	poBuff = new SA_Real[count * Stride];
	memset(poBuff, 0x0, TotalBufferSize);
}

void SpectralBuffer_Real::Destroy(AK::IAkPluginMemAlloc* alloc)
{
	AK_PLUGIN_FREE(alloc, poBuff);
}

void SpectralBuffer_Real::Destroy()
{
	delete[] poBuff;
}

void SpectralBuffer_Real::Clear()
{
	memset(poBuff, 0x0, TotalBufferSize);
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// SpectralBuffer_Complex implementation begins----------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

void SpectralBuffer_Complex::CopyPendingFrameData(
	AkReal32* frameData
	, AkUInt16 channelIndex
	, AkUInt32 frameCount
	, AkUInt32 offset
)
{
	for (
		AkUInt32 i = offset, end = offset + frameCount, inIndex = 0
		; i < end
		; ++i, ++inIndex
		)
	{
		poBuff[i] = (frameData[inIndex]), 0.0f;
	}
}

void SpectralBuffer_Complex::SumChannels(AkUInt8 channelCount)
{
	if (channelCount > 1)
	{
		SA_Complex* pSumBuffer = this->poBuff + (channelCount * Stride);

		memcpy_s(pSumBuffer, ChannelBufferSize, poBuff, ChannelBufferSize);

		for (AkUInt32 i = 1; i < channelCount; ++i)
		{
			SA_Complex* pCurChannel = poBuff + (i * Stride);

			for (AkUInt32 j = 0; j < Stride; ++j)
			{
				pSumBuffer[j] += pCurChannel[j];
			}
		}
	}
}

void SpectralBuffer_Complex::Pull(const SpectralBuffer* dataIn)
{
	memcpy_s(this->poBuff, TotalBufferSize, dataIn->GetBufferPosition(PullOffset), TotalBufferSize);
}

void* SpectralBuffer_Complex::GetBufferPosition(const AkUInt32& pullOffset) const
{
	return (void*)(poBuff + pullOffset);
}

void* SpectralBuffer_Complex::GetBufferPosition(RequestedChannel reqChannel) const
{
	return (void*)(poBuff + SAHelpers::GetPullOffset(Stride, reqChannel));
}

void SpectralBuffer_Complex::Setup(AK::IAkPluginMemAlloc& alloc, const AkUInt32& stride, RequestedChannel reqChannel)
{
	Stride = stride;
	PullOffset = SAHelpers::GetPullOffset(stride, reqChannel);
	ChannelBufferSize = stride * sizeof(SA_Complex);
	TotalBufferSize = ChannelBufferSize * SAHelpers::GetChannelCount(reqChannel);

	poBuff = static_cast<SA_Complex*>(AK_PLUGIN_ALLOC(&alloc, TotalBufferSize, 16));
	memset(poBuff, 0x0, TotalBufferSize);
}

void SpectralBuffer_Complex::Setup(const AkUInt32& stride, RequestedChannel reqChannel)
{
	Stride = stride;
	PullOffset = SAHelpers::GetPullOffset(stride, reqChannel);
	ChannelBufferSize = stride * sizeof(SA_Complex);
	AkUInt32 count = SAHelpers::GetChannelCount(reqChannel);
	TotalBufferSize = count * ChannelBufferSize;
	
	poBuff = new SA_Complex[count * stride];
	memset(poBuff, 0x0, TotalBufferSize);
}

void SpectralBuffer_Complex::Destroy(AK::IAkPluginMemAlloc* alloc)
{
	AK_PLUGIN_FREE(alloc, poBuff);
}

void SpectralBuffer_Complex::Destroy()
{
	delete[] poBuff;
}

void SpectralBuffer_Complex::Clear()
{
	memset(poBuff, 0x0, TotalBufferSize);
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// SAHelpers implementation begins----------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

AkInt8 SAHelpers::GetChannelCount(RequestedChannel req)
{
	switch (req)
	{
	case RequestedChannel::Sum:
	case RequestedChannel::Channel_1:
	case RequestedChannel::Channel_2:   return 1;
	case RequestedChannel::Channel_1_2: return 2;
	case RequestedChannel::All:         return 3;
	default:                            return 0;
	}
}

AkInt8 SAHelpers::GetDataCount(RequestedData req)
{
	switch (req)
	{
		break;
	case RequestedData::Spectrum:
	case RequestedData::Magnitude:
	case RequestedData::FFT:                    return 1;

	case RequestedData::Spectrum_Magnitude:
	case RequestedData::Spectrum_FFT:
	case RequestedData::Magnitude_FFT:          return 2;

	case RequestedData::Spectrum_Magnitude_FFT:
	case RequestedData::PushOnly:
	case RequestedData::All:                    return 3;

	default:                                    return 0;
	}
}

AkInt32 SAHelpers::GetPullOffset(AkUInt32 stride, RequestedChannel reqChannel)
{
	switch (reqChannel)
	{
	case RequestedChannel::Sum:			return stride * 2;
	case RequestedChannel::Channel_2:   return stride;
	
	case RequestedChannel::Channel_1:	
	case RequestedChannel::Channel_1_2: 
	case RequestedChannel::All:         
	default:                            return 0;
	}
}
