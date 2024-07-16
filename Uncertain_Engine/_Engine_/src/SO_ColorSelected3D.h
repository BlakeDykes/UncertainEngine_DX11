#pragma once

#include "ShaderObject.h"
#include "CBuffers.h"

class SO_ColorSelected3D : public ShaderObject
{
public:
	SO_ColorSelected3D();
	virtual ~SO_ColorSelected3D();

	virtual void SetToCurrent() override;

	// Inherited via ShaderObject
	virtual void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) override;
	virtual void ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos) override;

private:
	CB_ViewProj* poCB_VP;
	CB_WorldColor* poCB_WC;

protected:
	void GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context) override;
};


