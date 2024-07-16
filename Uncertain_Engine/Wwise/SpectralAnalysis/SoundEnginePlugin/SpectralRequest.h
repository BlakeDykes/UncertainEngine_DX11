#pragma once

#include "SpectralDAO.h"

class SpectralRequest
{
public:
	struct DataBundle
	{
		DataBundle(
			const SpectralBuffer_Complex& fft
			, const SpectralBuffer_Real& mag
			, const SpectralBuffer_Real& spec
		)
			: FFT(fft), Mag(mag), Spectrum(spec)
		{ }

		const SpectralBuffer_Complex& FFT;
		const SpectralBuffer_Real& Mag;
		const SpectralBuffer_Real& Spectrum;
	};

	SpectralRequest() = delete;
	SpectralRequest(const SpectralRequest&) = delete;
	SpectralRequest& operator=(const SpectralRequest&) = delete;
	virtual ~SpectralRequest() = default;

	SpectralRequest(RequestedChannel reqChannel);

	virtual void Execute(const DataBundle& data);

	RequestedChannel GetChannelReq() const { return this->ReqChannel; }

protected:
	RequestedChannel ReqChannel;
};

class SR_FFT : public SpectralRequest
{
public:
	SR_FFT(RequestedChannel reqChannel);

	virtual void Execute(const DataBundle& data) override final;

protected:
	virtual void HandleRequest(void* pBufferPos) = 0;
};

class SR_Mag : public SpectralRequest
{
public:
	SR_Mag(RequestedChannel reqChannel);

	virtual void Execute(const DataBundle& data) override final;

protected:
	virtual void HandleRequest(void* pBufferPos) = 0;
};

class SR_Spectrum : public SpectralRequest
{
public:
	SR_Spectrum(RequestedChannel reqChannel);

	virtual void Execute(const DataBundle& data) override final;

protected:
	virtual void HandleRequest(void* pBufferPos) = 0;

};