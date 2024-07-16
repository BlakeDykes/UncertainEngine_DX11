#pragma once

#include "ShaderObject.h"
#include "CBuffers.h"

class TextureObject;

class SO_SimpleTexture : public ShaderObject
{
public:
	SO_SimpleTexture();
	virtual ~SO_SimpleTexture();

	virtual void SetToCurrent() override;

	// Inherited via ShaderObject
	virtual void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) override;
	virtual void ReceiveCamData(const Mat4& view, const Mat4& proj, const Vec3& pos) override;

private:
	CB_ViewProj* poCB_VP;
	CB_World* poCB_World;

protected:
	// Inherited via ShaderObject
	void GenerateCBuffers(ID3D11Device* device, ID3D11DeviceContext* context) override;
};


