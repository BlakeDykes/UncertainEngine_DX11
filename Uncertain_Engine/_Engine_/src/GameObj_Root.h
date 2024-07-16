#pragma once

#include "GameObject.h"
#include "SceneLightObject.h"
#include "GameObj_Light.h"

class GraphicsObject;
struct PhongADS_Directional;
struct PhongADS_Point;
struct PhongADS_Spot;

class GameObj_Root : public GameObject
{
public:
	GameObj_Root(const char* const name, RenderOrder order);

	GameObject* Attach(GraphicsObject* graphicsObj, const char* const name = "GameObject", GameObject::Type objectType = GameObject::Type::Default);
	GameObj_Light* Attach(GraphicsObject* graphicsObj, SceneLightObject* sceneLight, PhongADS_Directional* pLight, const char* const name = "GameObject");
	GameObj_Light* Attach(GraphicsObject* graphicsObj, SceneLightObject* sceneLight, PhongADS_Point* pLight, const char* const name = "GameObject");
	GameObj_Light* Attach(GraphicsObject* graphicsObj, SceneLightObject* sceneLight, PhongADS_Spot* pLight, const char* const name = "GameObject");

	virtual void Render(ID3D11DeviceContext* context) override { AZUL_UNUSED_VAR(context); };

};
