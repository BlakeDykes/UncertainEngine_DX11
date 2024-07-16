#pragma once

#include "GameObject.h"
#include "SceneLightObject.h"

class GameObject_Parent : public GameObject
{
private:
	struct LightWorldUpdater;

	std::vector<GameObject*> GameObjects;
	std::vector<LightWorldUpdater*> Lights;
	SceneLightObject* pSceneLight;

public:
	GameObject_Parent(SceneLightObject* sceneLight);
	virtual ~GameObject_Parent();

	GameObject* AddGameObject(GraphicsObject*& go);
	void AddLight(LightType lType, size_t index);
	void AddLight(PhongADS_Directional& light);
	void AddLight(PhongADS_Point& light);
	void AddLight(PhongADS_Spot& light);

	virtual void Update() override;
	virtual void Render(ID3D11DeviceContext* context) override;
	virtual void ReflectedRender(ID3D11DeviceContext* context, Matrix* reflectionMatrix) override;

private:

	struct LightWorldUpdater
	{
		SceneLightObject* pSceneLight;
		size_t LightIndex;

		LightWorldUpdater(SceneLightObject* sceneLight, size_t index)
			: pSceneLight(sceneLight), LightIndex(index)
		{
		}

		virtual void SendLightWorldUpdate(const Matrix& mWorld) = 0;
	};

	struct DirUpdater : public LightWorldUpdater
	{
		DirUpdater(SceneLightObject* sceneLight, size_t index)
			: LightWorldUpdater(sceneLight, index)
		{ }

		virtual void SendLightWorldUpdate(const Matrix& mWorld) { pSceneLight->UpdateLightWorld_Dir(LightIndex, mWorld); }
	};
	struct PointUpdater : public LightWorldUpdater
	{
		PointUpdater(SceneLightObject* sceneLight, size_t index)
			: LightWorldUpdater(sceneLight, index)
		{ }

		virtual void SendLightWorldUpdate(const Matrix& mWorld) { pSceneLight->UpdateLightWorld_Point(LightIndex, mWorld); }
	};
	struct SpotUpdater : public LightWorldUpdater
	{
		SpotUpdater(SceneLightObject* sceneLight, size_t index)
			: LightWorldUpdater(sceneLight, index)
		{ }

		virtual void SendLightWorldUpdate(const Matrix& mWorld) { pSceneLight->UpdateLightWorld_Spot(LightIndex, mWorld); }
	};
};
