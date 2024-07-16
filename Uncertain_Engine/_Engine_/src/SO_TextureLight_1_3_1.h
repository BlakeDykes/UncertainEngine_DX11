#pragma once

#include "ShaderObject.h"
#include "CBuffers.h"

class SO_TextureLight_1_3_1 : public ShaderObject
{
public:
	SO_TextureLight_1_3_1();
	virtual ~SO_TextureLight_1_3_1();

	virtual void SetToCurrent() override;

	virtual void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) override;
	virtual void ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos) override;
	virtual void ReceiveLightData(const SceneLightObject& light) override;
	virtual void ReceiveLightData(const unsigned char& light) override;
	virtual void ReceiveFogData(const FogParams& fog) override;

private:
	CB_VPPos* poCB_VPPos;
	CB_WWInvMat* poCB_WWInvMat;
	CB_Raw* poCB_LightParams;
	CBuffer* poCB_FogParams;

protected:
	// Inherited via ShaderObject
	void GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context) override;
};


