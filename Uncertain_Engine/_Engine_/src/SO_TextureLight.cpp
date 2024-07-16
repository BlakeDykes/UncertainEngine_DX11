#include "SO_TextureLight.h"
#include "GameObject.h"
#include "SceneLightManager.h"

SO_TextureLight::SO_TextureLight()
    : ShaderObject(), poCB_VPPos(), poCB_WWInvMat(), poCB_LightParams(),
	poCB_FogParams()
{
}

SO_TextureLight::~SO_TextureLight()
{
    delete poCB_LightParams;
    delete poCB_WWInvMat;
    delete poCB_VPPos;
	delete poCB_FogParams;
}

void SO_TextureLight::SetToCurrent()
{
    ShaderObject::protSetToContext_VS_PS_IL();

    this->poCB_VPPos->SetToCurrent(pContext);
    this->poCB_WWInvMat->SetToCurrent(pContext);
    this->poCB_LightParams->SetToCurrent(pContext);
	this->poCB_FogParams->SetToCurrent(pContext);
}

void SO_TextureLight::UpdateData(const GameObject* pGO, const MaterialObject* pMaterial)
{
    this->poCB_WWInvMat->Set(*pGO->GetWorld(), *pMaterial->GetLightParams());
    this->poCB_WWInvMat->Update();
}

void SO_TextureLight::ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	this->poCB_VPPos->Set(view, proj, pos);
	this->poCB_VPPos->Update();
}

void SO_TextureLight::ReceiveLightData(const SceneLightObject& light)
{
	this->poCB_LightParams->Set(&light.GetPhongData());
	this->poCB_LightParams->Update();
}

void SO_TextureLight::ReceiveLightData(const unsigned char& light)
{
	this->poCB_LightParams->Set(&light);
	this->poCB_LightParams->Update();
}

void SO_TextureLight::ReceiveFogData(const FogParams& fog)
{
	this->poCB_FogParams->Update((const void*)&fog);
}

void SO_TextureLight::GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->poCB_VPPos = new CB_VPPos(device, context);
	this->poCB_WWInvMat = new CB_WWInvMat(device, context);
	this->poCB_LightParams = new CB_Raw(device, context, CBuffer::PIPELINE_STAGE::VERTEX_AND_PIXEL, SLOT_LightParameters, 1, SceneLightManager::GetActiveDataSize());
	this->poCB_FogParams = new CBuffer(device, context, CBuffer::PIPELINE_STAGE::PIXEL, SLOT_FogParams, 1, sizeof(FogParams));
}
