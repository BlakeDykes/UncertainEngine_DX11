#pragma once

#include "ShaderObject.h"

class SO_NULL : public ShaderObject
{
public:
	SO_NULL();

	virtual void SetToCurrent() override;

	void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) override;

protected:
	virtual HRESULT protCompile(ID3D11Device* device, ID3D11DeviceContext* context, ShaderName name) override;
	void GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context) override;

};


