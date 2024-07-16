#pragma once

#include "ManagerBase.h"
#include "_Time.h"
#include "SceneLightObject.h"
#include "LightParams.h"

class GraphicsObject;
class GameObject;

enum class LightSourceType
{
	Directional,
	Point,
	Spot
};

enum class LightColorType : BYTE
{
	Ambient  = 1,
	Diffuse  = 2,
	Specular = 4,
};

enum class LightSourceStatus : bool
{
	Inactive = false,
	Active   = true
};

class SceneLightManager : public ManagerBase
{
	static const int INITIAL_RESERVED = 1;
	static const int DELTA_GROW = 1;

public:
	static void Create(ID3D11Device*& device);
	static SceneLightObject* CreateSceneLightObj(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount, LightSourceStatus status);
	static SceneLightObject* CreateSceneLightObj(ShaderName shaderName, LightSourceStatus status);
	static void SetStatus(SceneLightObject* pLS, LightSourceStatus status);
	static void DestroyAll();

	static void Update(_Time frameTime);

	static SceneLightObject* GetActiveLightObj();
	static UINT GetActiveDataSize();

	static size_t GetIndex(PhongADS_Directional* light) { return Instance().pActive->GetIndex(light); }
	static size_t GetIndex(PhongADS_Point* light) { return Instance().pActive->GetIndex(light); }
	static size_t GetIndex(PhongADS_Spot* light) { return Instance().pActive->GetIndex(light); }

private:
	ID3D11Device* pDevice;
	SceneLightObject* pActive;

	SceneLightManager();
	SceneLightManager(const SceneLightManager&) = delete;
	SceneLightManager& operator =(const SceneLightManager&) = delete;

	static SceneLightManager& Instance();

	// Inherited via ManagerBase
	NodeBase* CreateNode() override;
};

