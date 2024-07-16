#pragma once

#include "ShaderObject.h"
#include "CBuffers.h"
#include "SpectralRequest_CBuffer.h"

class SO_SpectrumLight : public ShaderObject
{
public:
	SO_SpectrumLight();
	virtual ~SO_SpectrumLight();

	virtual void SetToCurrent() override;

	virtual void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) override;
	virtual void ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos) override;
	virtual void ReceiveLightData(const SceneLightObject& light) override;
	virtual void ReceiveLightData(const unsigned char& light) override;

private:
	CB_VPPos* poCB_VPPos;
	CB_WWInvMat* poCB_WWInvMat;
	CB_Raw*  poCB_LightParams;
	SpecReq_CB_Spectrum* poCB_SpectrumData;

protected:
	// Inherited via ShaderObject
	void GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context) override;
};


