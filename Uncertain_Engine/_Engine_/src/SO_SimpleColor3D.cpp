#include "SO_SimpleColor3D.h"
#include "GameObject.h"

SO_SimpleColor3D::SO_SimpleColor3D()
	: ShaderObject(), poCB_VPW()
{
}

SO_SimpleColor3D::~SO_SimpleColor3D()
{
	delete poCB_VPW;
}

void SO_SimpleColor3D::SetToCurrent()
{
	ShaderObject::protSetToContext_VS_PS_IL();

	this->poCB_VPW->SetToCurrent(pContext);
}

void SO_SimpleColor3D::UpdateData(const GameObject* pGO, const MaterialObject* pMaterial)
{
	ALIGN_UNUSED_VAR(pMaterial);
	this->poCB_VPW->SetWorld(*pGO->GetWorld());
	this->poCB_VPW->Update();
}

void SO_SimpleColor3D::ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	ALIGN_UNUSED_VAR(pos);

	this->poCB_VPW->SetViewProj(view, proj);
	this->poCB_VPW->Update();
}

void SO_SimpleColor3D::GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->poCB_VPW = new CB_ViewProjWorld(device, context);
}


