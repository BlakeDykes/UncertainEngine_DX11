#include "SO_ColorSelected3D.h"
#include "GameObject.h"
#include "MaterialObject.h"

SO_ColorSelected3D::SO_ColorSelected3D()
    : ShaderObject(), poCB_VP(), poCB_WC()
{
}

SO_ColorSelected3D::~SO_ColorSelected3D()
{
    delete poCB_WC;
    delete poCB_VP;
}

void SO_ColorSelected3D::UpdateData(const GameObject* pGO, const MaterialObject* pMaterial)
{
    this->poCB_WC->Set(*pGO->GetWorld(), pMaterial->GetAmbient());
    this->poCB_WC->Update();
}

void SO_ColorSelected3D::SetToCurrent()
{
	ShaderObject::protSetToContext_VS_PS_IL();

	this->poCB_VP->SetToCurrent(pContext);
	this->poCB_WC->SetToCurrent(pContext);
}

void SO_ColorSelected3D::ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos)
{
	ALIGN_UNUSED_VAR(pos);
    this->poCB_VP->Set(view, proj);
    this->poCB_VP->Update();
}

void SO_ColorSelected3D::GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
	this->poCB_VP = new CB_ViewProj(device, context);
	this->poCB_WC = new CB_WorldColor(device, context);
}
