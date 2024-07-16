#include "SpectralRequest_CBuffer.h"
#include "CBuffers.h"

namespace Uncertain
{
	SpecReq_CB_FFT::SpecReq_CB_FFT(ID3D11Device* device, ID3D11DeviceContext* context, RequestedChannel reqChannel)
		: SR_FFT(reqChannel), poBuffer(new CB_SA_FFT(device, context, SAHelpers::GetChannelCount(reqChannel)))
	{
	}

	SpecReq_CB_FFT::~SpecReq_CB_FFT()
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		delete this->poBuffer;
	}

	void SpecReq_CB_FFT::SetToCurrent(ID3D11DeviceContext* context)
	{
		this->poBuffer->SetToCurrent(context);
	}

	void SpecReq_CB_FFT::Update()
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		this->poBuffer->Update();
	}

	void SpecReq_CB_FFT::HandleRequest(void* pBufferPos)
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		this->poBuffer->Update(pBufferPos);
	}

	SpecReq_CB_Mag::SpecReq_CB_Mag(ID3D11Device* device, ID3D11DeviceContext* context, RequestedChannel reqChannel)
		: SR_Mag(reqChannel), poBuffer(new CB_SA_Mag(device, context, SAHelpers::GetChannelCount(reqChannel)))
	{
	}

	SpecReq_CB_Mag::~SpecReq_CB_Mag()
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		delete this->poBuffer;
	}

	void SpecReq_CB_Mag::Update()
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		this->poBuffer->Update();
	}

	void SpecReq_CB_Mag::SetToCurrent(ID3D11DeviceContext* context)
	{
		this->poBuffer->SetToCurrent(context);
	}

	void SpecReq_CB_Mag::HandleRequest(void* pBufferPos)
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		this->poBuffer->Update(pBufferPos);
	}

	SpecReq_CB_Spectrum::SpecReq_CB_Spectrum(ID3D11Device* device, ID3D11DeviceContext* context, RequestedChannel reqChannel)
		: SR_Spectrum(reqChannel), poBuffer(new CB_SA_Spectrum(device, context, SAHelpers::GetChannelCount(reqChannel)))
	{
	}

	SpecReq_CB_Spectrum::~SpecReq_CB_Spectrum()
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		delete this->poBuffer;
	}

	void SpecReq_CB_Spectrum::Update()
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		this->poBuffer->Update();
	}

	void SpecReq_CB_Spectrum::SetToCurrent(ID3D11DeviceContext* context)
	{
		this->poBuffer->SetToCurrent(context);
	}

	void SpecReq_CB_Spectrum::HandleRequest(void* pBufferPos)
	{
		std::lock_guard<std::timed_mutex> lg(mtx);
		this->poBuffer->Update(pBufferPos);
	}

}
