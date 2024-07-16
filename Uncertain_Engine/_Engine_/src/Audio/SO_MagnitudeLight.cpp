#include "SO_MagnitudeLight.h"
#include "GameObject.h"
#include "SceneLightManager.h"
#include "AudioManager.h"
//#include "GameObj_Audio.h"

SO_MagnitudeLight::SO_MagnitudeLight()
	: ShaderObject()
	, poCB_VPPos(nullptr)
	, poCB_WWInvMat(nullptr)
	, poCB_LightParams(nullptr)
	, poCB_MagData(nullptr)
{
}

SO_MagnitudeLight::~SO_MagnitudeLight()
{
	delete poCB_LightParams;
	delete poCB_WWInvMat;
	delete poCB_VPPos;
	delete poCB_MagData;
}

void SO_MagnitudeLight::SetToCurrent()
{
	ShaderObject::protSetToContext_VS_PS_IL();

	this->poCB_VPPos->SetToCurrent(pContext);
	this->poCB_WWInvMat->SetToCurrent(pContext);
	this->poCB_LightParams->SetToCurrent(pContext);
	this->poCB_MagData->SetToCurrent(pContext);
}

void SO_MagnitudeLight::UpdateData(const GameObject* pGO, const MaterialObject* pMaterial)
{
	//const GameObj_Audio* pAGO = static_cast<const GameObj_Audio*>(pGO);

	this->poCB_WWInvMat->Set(*pGO->GetWorld(), *pMaterial->GetLightParams());
	this->poCB_WWInvMat->Update();

	this->poCB_MagData->Update();
}

void SO_MagnitudeLight::ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	this->poCB_VPPos->Set(view, proj, pos);
	this->poCB_VPPos->Update();
}

void SO_MagnitudeLight::ReceiveLightData(const SceneLightObject& light)
{
	this->poCB_LightParams->Set(&light.GetPhongData());
	this->poCB_LightParams->Update();
}

void SO_MagnitudeLight::ReceiveLightData(const unsigned char& light)
{
	this->poCB_LightParams->Set(&light);
	this->poCB_LightParams->Update();
}

void SO_MagnitudeLight::GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->poCB_VPPos = new CB_VPPos(device, context);
	this->poCB_WWInvMat = new CB_WWInvMat(device, context);
	this->poCB_LightParams = new CB_Raw(device, context, CBuffer::PIPELINE_STAGE::VERTEX_AND_PIXEL, SLOT_LightParameters, 1, SceneLightManager::GetActiveDataSize());
	this->poCB_MagData = new SpecReq_CB_Mag(device, context, RequestedChannel::Channel_1);

	AudioManager::AddGlobalSpectralRequest(poCB_MagData);
}
