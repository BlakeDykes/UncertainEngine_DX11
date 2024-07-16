#include "CBuffers.h"
#include "Enum.h"
#include "SA_FFT.h"

CBuffer::CBuffer()
	: poInstBuffer(nullptr), pContext(nullptr), PipelineStage(UNDEFINED),
	StartSlot(0), NumBuffers(0)
{
}

CBuffer::~CBuffer()
{
	ReleaseAndDeleteCOMobject(poInstBuffer);
}

CBuffer::CBuffer(ID3D11Device* device, ID3D11DeviceContext* context,
	PIPELINE_STAGE stage, UINT startSlot, UINT numBuffers, UINT byteWidth)
	: poInstBuffer(nullptr), pContext(context), PipelineStage(stage),
	StartSlot(startSlot), NumBuffers(numBuffers)
{
	// Constant Buffer preperation
	// Create constant buffers for View/Proj matrix and instance data.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = byteWidth;

	HRESULT hr = device->CreateBuffer(&bd, nullptr, &poInstBuffer);
	if (SUCCEEDED(hr))
	{
		char* temp = new char[byteWidth] { 0 };

		context->UpdateSubresource(poInstBuffer, 0, nullptr, temp, 0, 0);

		delete[] temp;

		SetToCurrent(context);
	}
}

void CBuffer::Update(const void* pData)
{
	protUpdate(pData);
}

void CBuffer::SetToCurrent(ID3D11DeviceContext* context)
{
	switch (PipelineStage)
	{
	case CBuffer::VERTEX:
		context->VSSetConstantBuffers(StartSlot, NumBuffers, &poInstBuffer);
		break;
	case CBuffer::PIXEL:
		context->PSSetConstantBuffers(StartSlot, NumBuffers, &poInstBuffer);
		break;
	case CBuffer::VERTEX_AND_PIXEL:
		context->VSSetConstantBuffers(StartSlot, NumBuffers, &poInstBuffer);
		context->PSSetConstantBuffers(StartSlot, NumBuffers, &poInstBuffer);
		break;
	default:
		break;
	}
}

void CBuffer::protUpdate(const void* d) 
{ 
	pContext->UpdateSubresource(poInstBuffer, 0, nullptr, d, 0, 0); 
}


// ---------------------------------------------------------------------------------
// ---- CB_ViewProjData
// ---------------------------------------------------------------------------------
CB_ViewProjWorld::CB_ViewProjWorld(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX, SLOT_ViewProj, 1, sizeof(CB_ViewProjWorld::BufferData)),
	_Data()
{
}

void CB_ViewProjWorld::Update()
{
	protUpdate(&_Data);
}

void CB_ViewProjWorld::SetViewProj(const Mat4& view, const Mat4& proj)
{
	_Data.mView = view;
	_Data.mProj = proj;
}

void CB_ViewProjWorld::SetWorld(const Mat4& world)
{
	_Data.mWorld = world;
}

void CB_ViewProjWorld::Set(const Mat4& view, const Mat4& proj, const Mat4& world)
{
	_Data.mView = view;
	_Data.mProj = proj;
	_Data.mWorld = world;
}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// ---- CB_ViewProjData
// ---------------------------------------------------------------------------------
CB_ViewProj::CB_ViewProj(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX, SLOT_ViewProj, 1, sizeof(CB_ViewProj::BufferData)),
	_Data()
{
}

void CB_ViewProj::Update()
{
	protUpdate(&_Data);
}

void CB_ViewProj::Set(const Mat4& view, const Mat4& proj)
{
	_Data.mView = view;
	_Data.mProj = proj;
}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_WorldColor
// ---------------------------------------------------------------------------------

CB_WorldColor::CB_WorldColor(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX, SLOT_ColorSelected3D_Inst, 1, sizeof(CB_WorldColor::BufferData)),
	_Data()
{
}

void CB_WorldColor::Update()
{
	protUpdate(&_Data);
}

void CB_WorldColor::Set(const Mat4& world, const Vec4& color)
{
	_Data.mWorld = world;
	_Data.vColor = color;
}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// ---- CB_ViewProjWorld_PreCalc
// ---------------------------------------------------------------------------------
CB_ViewProjWorld_PreCalc::CB_ViewProjWorld_PreCalc(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX, SLOT_ViewProj, 1, sizeof(CB_ViewProjWorld_PreCalc::BufferData)),
	_Data()
{
}

void CB_ViewProjWorld_PreCalc::Update()
{
	_Data.mWVP = mWorld * mView * mProj;
	protUpdate(&_Data);
}

void CB_ViewProjWorld_PreCalc::SetViewProj(const Mat4& view, const Mat4& proj)
{
	mView = view;
	mProj = proj;
}

void CB_ViewProjWorld_PreCalc::SetWorld(const Mat4& world)
{
	mWorld = world;
}

void CB_ViewProjWorld_PreCalc::Set(const Mat4& view, const Mat4& proj, const Mat4& world)
{
	mView = view;
	mProj = proj;
	mWorld = world;
}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_Color
// ---------------------------------------------------------------------------------
CB_Color::CB_Color(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX, SLOT_ColorSelected3D_Inst, 1, sizeof(CB_Color::BufferData)),
	_Data()
{
}

void CB_Color::Update()
{
	protUpdate(&_Data);
}

void CB_Color::Set(const Vec4& color)
{
	this->_Data.vColor = color;
}

// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------



// ---------------------------------------------------------------------------------
// ---- CB_World
// ---------------------------------------------------------------------------------
CB_World::CB_World(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX, SLOT_World, 1, sizeof(CB_World::BufferData)),
	_Data()
{
}

void CB_World::Update()
{
	protUpdate(&_Data);
}

void CB_World::Set(const Mat4& world)
{
	this->_Data.mWorld = world;
}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------


// ---------------------------------------------------------------------------------
// ---- CB_VPPos
// ---------------------------------------------------------------------------------
CB_VPPos::CB_VPPos(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX_AND_PIXEL, SLOT_ViewProj, 1, sizeof(CB_VPPos::BufferData)),
	_Data()
{
}

void CB_VPPos::Update()
{
	protUpdate(&_Data);
}

void CB_VPPos::Set(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	this->_Data.mView = view;
	this->_Data.mProj = proj;
	this->_Data.vEyePosWorld = pos;
}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_WWInvMat
// ---------------------------------------------------------------------------------

CB_WWInvMat::CB_WWInvMat(ID3D11Device* device, ID3D11DeviceContext* context)
	: CBuffer(device, context, VERTEX_AND_PIXEL, SLOT_World, 1, sizeof(CB_WWInvMat::BufferData)),
	_Data()
{
}

void CB_WWInvMat::Update()
{
	protUpdate(&_Data);
}

void CB_WWInvMat::Set(const Mat4& world, const MaterialObject::MatLightParameters& ads)
{
	this->_Data.mWorld = world;
	this->_Data.mWorldInv = _Data.mWorld.getInv();
	memcpy_s((char*)(&_Data) + sizeof(Mat4) * 2, sizeof(CB_WWInvMat::BufferData), &ads, sizeof(MaterialObject::MatLightParameters));

}
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------
// ---- CB_Raw
// ---------------------------------------------------------------------------------

CB_Raw::CB_Raw(ID3D11Device* device, ID3D11DeviceContext* context, PIPELINE_STAGE stage, UINT startSlot, UINT numBuffers, UINT byteWidth)
	: CBuffer(device, context, stage, startSlot, numBuffers, byteWidth), _Data(nullptr)
{
}

void CB_Raw::Update()
{
	protUpdate(_Data);
}

void CB_Raw::Set(const unsigned char* const pData)
{
	this->_Data = pData;
}


// ---------------------------------------------------------------------------------
// ---- CB_SA_Spectrum
// ---------------------------------------------------------------------------------

CB_SA_Spectrum::CB_SA_Spectrum(ID3D11Device* device, ID3D11DeviceContext* context, uint8_t channelCount)
	: CBuffer(device, context, VERTEX_AND_PIXEL, SLOT_SA_AudioData, 1, channelCount * FFT::SPECTRUM_BUFFER_BITS)
	, BufferSize(channelCount * FFT::SPECTRUM_BUFFER_BITS)
	, poData((void*)new unsigned char[BufferSize])
{
	memset(poData, 0x0, BufferSize);
}

CB_SA_Spectrum::~CB_SA_Spectrum()
{
	delete[] poData;
}

void CB_SA_Spectrum::Update()
{
	protUpdate(poData);
}

void CB_SA_Spectrum::Update(const void* pData)
{
	memcpy_s(poData, BufferSize, pData, BufferSize);
}

// ---------------------------------------------------------------------------------
// ---- CB_SA_Mag
// ---------------------------------------------------------------------------------

CB_SA_Mag::CB_SA_Mag(ID3D11Device* device, ID3D11DeviceContext* context, uint8_t channelCount)
	: CBuffer(device, context, VERTEX, SLOT_SA_AudioData, 1, channelCount * FFT::MAG_BUFFER_BITS)
	, BufferSize(channelCount* FFT::MAG_BUFFER_BITS)
	, poData((void*)new unsigned char[BufferSize])
{
	memset(poData, 0x0, BufferSize);
}

CB_SA_Mag::~CB_SA_Mag()
{
	delete[] poData;
}

void CB_SA_Mag::Update()
{
	protUpdate(poData);
}

void CB_SA_Mag::Update(const void* pData)
{
	memcpy_s(poData, BufferSize, pData, BufferSize);
}

// ---------------------------------------------------------------------------------
// ---- CB_SA_FFT
// ---------------------------------------------------------------------------------

CB_SA_FFT::CB_SA_FFT(ID3D11Device* device, ID3D11DeviceContext* context, uint8_t channelCount)
	: CBuffer(device, context, VERTEX_AND_PIXEL, SLOT_SA_AudioData, 1, channelCount* FFT::FRAME_BUFFER_BITS)
	, BufferSize(channelCount * FFT::FRAME_BUFFER_BITS)
	, poData((void*)new unsigned char[BufferSize])
{
	memset(poData, 0x0, BufferSize);
}

CB_SA_FFT::~CB_SA_FFT()
{
	delete[] poData;
}

void CB_SA_FFT::Update()
{
	protUpdate(poData);
}

void CB_SA_FFT::Update(const void* pData)
{
	memcpy_s(poData, BufferSize, pData, BufferSize);
}
