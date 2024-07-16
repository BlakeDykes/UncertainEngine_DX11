#pragma once

#include <d3d11.h>
#include "d3dUtil.h"
#include "Mat4.h"
#include "MaterialObject.h"

static const UINT SLOT_ViewProj				= 0;
static const UINT SLOT_ColorSelected3D_Inst = 1;
static const UINT SLOT_World				= 1;
static const UINT SLOT_LightParameters		= 2;
static const UINT SLOT_FogParams			= 3;
static const UINT SLOT_SA_AudioData			= 4;


struct CBuffer : public Align16
{
	enum PIPELINE_STAGE
	{
		VERTEX,
		PIXEL,
		VERTEX_AND_PIXEL,

		UNDEFINED
	};

	CBuffer();
	CBuffer(ID3D11Device* device, ID3D11DeviceContext* context,
		PIPELINE_STAGE stage, UINT startSlot, UINT numBuffers, UINT byteWidth);
	virtual ~CBuffer();

	virtual void Update() {}
	virtual void Update(const void* pData);

	void SetToCurrent(ID3D11DeviceContext* context);

protected:
	ID3D11Buffer* poInstBuffer;
	ID3D11DeviceContext* pContext;

	PIPELINE_STAGE PipelineStage;
	UINT StartSlot;
	UINT NumBuffers;

	virtual void protUpdate(const void* d);
};

// ---------------------------------------------------------------------------------
// ---- CB_ViewProjWorld
// ---------------------------------------------------------------------------------
struct CB_ViewProjWorld : CBuffer
{
	struct BufferData
	{
		Mat4 mWorld;
		Mat4 mView;
		Mat4 mProj;

		BufferData()
			:mView(Mat4::IDENTITY_MAT4), mProj(Mat4::IDENTITY_MAT4), mWorld(Mat4::IDENTITY_MAT4)
		{
		}
	};

	BufferData _Data;

	CB_ViewProjWorld(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Update() override;
	void SetViewProj(const Mat4& view, const Mat4& proj);
	void SetWorld(const Mat4& world);
	void Set(const Mat4& view, const Mat4& proj, const Mat4& world);

};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// ---- CB_ViewProjWorld_PreCalc
// ---------------------------------------------------------------------------------
struct CB_ViewProjWorld_PreCalc : CBuffer
{
	struct BufferData
	{
		Mat4 mWVP;

		BufferData()
			:mWVP(Mat4::IDENTITY_MAT4)
		{
		}
	};

	Mat4 mView;
	Mat4 mProj;
	Mat4 mWorld;

	BufferData _Data;

	CB_ViewProjWorld_PreCalc(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Update() override;
	void SetViewProj(const Mat4& view, const Mat4& proj);
	void SetWorld(const Mat4& world);
	void Set(const Mat4& view, const Mat4& proj, const Mat4& world);

};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------
// ---- CB_ViewProj
// ---------------------------------------------------------------------------------
struct CB_ViewProj : CBuffer
{
	struct BufferData
	{
		Mat4 mView;
		Mat4 mProj;

		BufferData()
			:mView(Mat4::IDENTITY_MAT4), mProj(Mat4::IDENTITY_MAT4)
		{
		}
	};

	BufferData _Data;

	CB_ViewProj(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Update() override;
	void Set(const Mat4& view, const Mat4& proj);

};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_WorldColor
// ---------------------------------------------------------------------------------
struct CB_WorldColor : public CBuffer
{
	struct BufferData
	{
		Mat4 mWorld;
		Vec4 vColor;
	};

	BufferData _Data;

	CB_WorldColor(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Update() override;
	void Set(const Mat4& world, const Vec4& color);
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_World
// ---------------------------------------------------------------------------------
struct CB_World : public CBuffer
{
	struct BufferData
	{
		Mat4 mWorld;
	};

	BufferData _Data;

	CB_World(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Update() override;
	void Set(const Mat4& world);
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// ---- CB_Color
// ---------------------------------------------------------------------------------
struct CB_Color : public CBuffer
{
	struct BufferData
	{
		Vec4 vColor;
	};

	BufferData _Data;

	CB_Color(ID3D11Device* device, ID3D11DeviceContext* context);

	virtual void Update() override;
	void Set(const Vec4& color);
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_VPPos
// ---------------------------------------------------------------------------------
struct CB_VPPos : public CBuffer
{
	struct BufferData
	{
		Mat4 mView;
		Mat4 mProj;
		Vec3 vEyePosWorld;
	};

	CB_VPPos(ID3D11Device* device, ID3D11DeviceContext* context);

	BufferData _Data;

	virtual void Update() override;
	void Set(const Mat4& view, const Mat4& proj, const Vec3& pos);
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_WWInvMat
// ---------------------------------------------------------------------------------
struct CB_WWInvMat : public CBuffer
{
	struct BufferData
	{
		Mat4 mWorld;
		Mat4 mWorldInv;
		Vec4 Ambient;
		Vec4 Diffuse;
		Vec4 Specular;
	};

	CB_WWInvMat(ID3D11Device* device, ID3D11DeviceContext* context);

	BufferData _Data;

	virtual void Update() override;
	void Set(const Mat4& world, const MaterialObject::MatLightParameters& ads);
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_Raw
// ---------------------------------------------------------------------------------
struct CB_Raw : public CBuffer
{
	CB_Raw(ID3D11Device* device, ID3D11DeviceContext* context,
		PIPELINE_STAGE stage, UINT startSlot, UINT numBuffers, UINT byteWidth);

	const unsigned char* _Data;
	
	virtual void Update() override;
	void Set(const unsigned char* const pData);
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_SA_Spectrum
// ---------------------------------------------------------------------------------
struct CB_SA_Spectrum : public CBuffer
{
	CB_SA_Spectrum(ID3D11Device* device, ID3D11DeviceContext* context, uint8_t channelCount = 1);
	virtual ~CB_SA_Spectrum();

	virtual void Update() override;
	virtual void Update(const void* pData);

	UINT BufferSize;
	void* poData;
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_SA_Mag
// ---------------------------------------------------------------------------------
struct CB_SA_Mag : public CBuffer
{
	CB_SA_Mag(ID3D11Device* device, ID3D11DeviceContext* context, uint8_t channelCount = 1);
	virtual ~CB_SA_Mag();

	virtual void Update() override;
	virtual void Update(const void* pData);

	UINT BufferSize;
	void* poData;
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_SA_FFT
// ---------------------------------------------------------------------------------
struct CB_SA_FFT : public CBuffer
{
	CB_SA_FFT(ID3D11Device* device, ID3D11DeviceContext* context, uint8_t channelCount = 1);
	virtual ~CB_SA_FFT();

	virtual void Update() override;
	virtual void Update(const void* pData);

	UINT BufferSize;
	void* poData;
};
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
