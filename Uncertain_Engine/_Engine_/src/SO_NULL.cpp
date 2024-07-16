#include "SO_NULL.h"

SO_NULL::SO_NULL()
    : ShaderObject()
{
}

void SO_NULL::SetToCurrent()
{
}

void SO_NULL::UpdateData(const GameObject* pGO, const MaterialObject* pMaterial)
{
    ALIGN_UNUSED_VAR(pGO);
    ALIGN_UNUSED_VAR(pMaterial);
}

HRESULT SO_NULL::protCompile(ID3D11Device* device, ID3D11DeviceContext* context, ShaderName name)
{
    ALIGN_UNUSED_VAR(device);
    ALIGN_UNUSED_VAR(context);
    ALIGN_UNUSED_VAR(name);

    return E_FAIL;
}

void SO_NULL::GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context)
{
    ALIGN_UNUSED_VAR(device);
    ALIGN_UNUSED_VAR(context);
}