#pragma once

#include "GameObject.h"
#include "SceneLightManager.h"

class GameObj_Light : public GameObject
{
private:
	struct LightUpdateMessenger;

	LightUpdateMessenger* poLightM;

public:

	GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order);
	GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order, SceneLightObject* sceneLight, PhongADS_Directional* pLight);
	GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order, SceneLightObject* sceneLight, PhongADS_Point* pLight);
	GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order, SceneLightObject* sceneLight, PhongADS_Spot* pLight);

	virtual ~GameObj_Light();

	virtual void Update() override;

	void Set(SceneLightObject* sceneLight, PhongADS_Directional* pLight);
	void Set(SceneLightObject* sceneLight, PhongADS_Point* pLight);
	void Set(SceneLightObject* sceneLight, PhongADS_Spot* pLight);

private:

	struct LightUpdateMessenger
	{
		SceneLightObject* pSceneLight;
		size_t LightIndex;
		float SpecCache;

		LightUpdateMessenger(SceneLightObject* sceneLight, size_t index)
			: pSceneLight(sceneLight), LightIndex(index), SpecCache(0.0f)
		{ }

		virtual ~LightUpdateMessenger() = default;

		virtual void SendWorld(const Mat4& vLocalRot, const Quat& mThisRot, Mat4& mWorld) = 0;
		virtual void SendTurnOff() = 0;
		virtual void SendTurnOn() = 0;
	};

	struct Dir_Messenger : public LightUpdateMessenger
	{

		Dir_Messenger(SceneLightObject* sceneLight, size_t index);

		virtual void SendWorld(const Mat4& localRot, const Quat& mThisRot, Mat4& mWorld) override;
		virtual void SendTurnOff() override { pSceneLight->TurnOff_Dir(LightIndex, SpecCache); }
		virtual void SendTurnOn() override { pSceneLight->TurnOn_Spot(LightIndex, SpecCache); }
	};
	struct Point_Messenger : public LightUpdateMessenger
	{
		Point_Messenger(SceneLightObject* sceneLight, size_t index);

		virtual void SendWorld(const Mat4& vLocalRot, const Quat& mThisRot, Mat4& mWorld) override;
		virtual void SendTurnOff() override { pSceneLight->TurnOff_Point(LightIndex, SpecCache); }
		virtual void SendTurnOn() override { pSceneLight->TurnOn_Point(LightIndex, SpecCache); }
	};
	struct Spot_Messenger : public LightUpdateMessenger
	{
		
		Spot_Messenger(SceneLightObject* sceneLight, size_t index);

		virtual void SendWorld(const Mat4& vLocalRot, const Quat& mThisRot, Mat4& mWorld) override;
		virtual void SendTurnOff() override { pSceneLight->TurnOff_Dir(LightIndex, SpecCache); }
		virtual void SendTurnOn() override { pSceneLight->TurnOn_Spot(LightIndex, SpecCache); }
	};
};

