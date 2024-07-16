#include "SO_SimpleTexture.h"
#include "TextureObject.h"
#include "GameObject.h"

SO_SimpleTexture::SO_SimpleTexture()
	: ShaderObject(), poCB_VP(nullptr), poCB_World(nullptr)
{
}

SO_SimpleTexture::~SO_SimpleTexture()
{
	delete this->poCB_World;
	delete this->poCB_VP;
}

void SO_SimpleTexture::SetToCurrent()
{
	ShaderObject::protSetToContext_VS_PS_IL();
	this->poCB_VP->SetToCurrent(pContext);
	this->poCB_World->SetToCurrent(pContext);
}

void SO_SimpleTexture::UpdateData(const GameObject* pGO, const MaterialObject* pMaterial)
{
	ALIGN_UNUSED_VAR(pMaterial);
	this->poCB_World->Set(*pGO->GetWorld());
	this->poCB_World->Update();
}

void SO_SimpleTexture::ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	ALIGN_UNUSED_VAR(pos);

	this->poCB_VP->Set(view, proj);
	this->poCB_VP->Update();
}

void SO_SimpleTexture::GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->poCB_VP = new CB_ViewProj(device, context);
	this->poCB_World = new CB_World(device, context);
}
