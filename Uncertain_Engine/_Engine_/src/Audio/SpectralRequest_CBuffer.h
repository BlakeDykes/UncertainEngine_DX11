#pragma once

#include <d3d11.h>
#include "d3dUtil.h"
#include "SpectralRequest.h"
#include <mutex>

struct CB_SA_Spectrum;
struct CB_SA_Mag;
struct CB_SA_FFT;

namespace Uncertain
{
	class SpecReq_CB_FFT : public SR_FFT
	{
	public:
		SpecReq_CB_FFT(ID3D11Device* device, ID3D11DeviceContext* context, RequestedChannel reqChannel);
		virtual ~SpecReq_CB_FFT();

		void SetToCurrent(ID3D11DeviceContext* context);

		// Pushes to GPU
		void Update();

	protected:
		std::timed_mutex mtx;
		CB_SA_FFT* poBuffer;

		// Memcpy pDataIn
		virtual void HandleRequest(void* pDataIn) override;
	};

	class SpecReq_CB_Mag : public SR_Mag
	{
	public:

		SpecReq_CB_Mag(ID3D11Device* device, ID3D11DeviceContext* context, RequestedChannel reqChannel);
		virtual ~SpecReq_CB_Mag();

		void SetToCurrent(ID3D11DeviceContext* context);
		
		// Pushes to GPU
		void Update();

	protected:
		std::timed_mutex mtx;
		CB_SA_Mag* poBuffer;

		// memcpy pDataIn
		virtual void HandleRequest(void* pBufferPos) override;
	};

	class SpecReq_CB_Spectrum : public SR_Spectrum
	{
	public:

		SpecReq_CB_Spectrum(ID3D11Device* device, ID3D11DeviceContext* context, RequestedChannel reqChannel);
		virtual ~SpecReq_CB_Spectrum();

		void SetToCurrent(ID3D11DeviceContext* context);
		
		// Pushes to GPU 
		void Update();

	protected:
		std::timed_mutex mtx;
		CB_SA_Spectrum* poBuffer;

		// memcpy pDataIn
		virtual void HandleRequest(void* pDataIn) override;
	};

}