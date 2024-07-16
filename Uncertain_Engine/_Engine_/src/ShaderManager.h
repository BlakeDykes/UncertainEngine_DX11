#pragma once

#include "ManagerBase.h"
#include "ShaderNode.h"
#include "TextureObject.h"
#include "Camera.h"
#include "Mat4.h"

struct LightCount;

class ShaderManager : public ManagerBase
{
	static const int INITIAL_RESERVED = 10;
	static const int DELTA_GROW = 10;

public:
	ShaderManager();
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager& operator =(const ShaderManager&) = delete;
	virtual ~ShaderManager();

	static void Create(ID3D11Device*& device);
	static void DestroyAll();

	static ShaderNode* CreateShader(ID3D11DeviceContext* context, ShaderName name);
	static ShaderNode* FindShader(ShaderName name);

	static void SetCameraAndLighting(Camera& cam);
	static void SetLighting();
	static void SetReflectedLighting(const Mat4& reflectionMat);
	static void SetFog(const FogParams& fog);
	static void SetToCurrent(ShaderNode*& so);
	static LightCount GetMaxLightCount(ShaderName shaderName);

protected:
	ShaderManager(ID3D11Device*& device);

	ID3D11Device* pDevice;
	NodeBase::Handle CurrentSO;

	static ShaderManager* instance;
	static ShaderManager& Instance();

private:
	ShaderNode* poComp;

	// Inherited via ManagerBase
	NodeBase* CreateNode() override;
};