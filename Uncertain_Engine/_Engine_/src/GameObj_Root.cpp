#include "GameObj_Root.h"
#include "GraphicsObject.h"
#include "GameObjectManager.h"
#include "LightParams.h"

GameObj_Root::GameObj_Root(const char* const name, RenderOrder order)
	: GameObject(nullptr, name, order)
{
}

GameObject* GameObj_Root::Attach(GraphicsObject* graphicsObj, const char* const name, GameObject::Type objectType)
{
	switch (ROrder)
	{
	case GameObject::RenderOrder::Default: 		return GameObjectManager::Add(graphicsObj, name, this, objectType);
	case GameObject::RenderOrder::Reflected:	return GameObjectManager::Add_Reflected(this->pReflectionParent, graphicsObj, name, this, objectType);
	case GameObject::RenderOrder::Deferred:		return GameObjectManager::Add_Deferred(this->pReflectionParent, graphicsObj, name, this, objectType);
	default:
		assert(false);
		return nullptr;
	}
}

GameObj_Light* GameObj_Root::Attach(GraphicsObject* graphicsObj, SceneLightObject* sceneLight, PhongADS_Directional* pLight, const char* const name)
{
	GameObj_Light* pObj = (GameObj_Light*)GameObjectManager::Add(graphicsObj, name, this, Type::Light);
	pObj->Set(sceneLight, pLight);
	return pObj;
}

GameObj_Light* GameObj_Root::Attach(GraphicsObject* graphicsObj, SceneLightObject* sceneLight, PhongADS_Point* pLight, const char* const name)
{
	GameObj_Light* pObj = (GameObj_Light*)GameObjectManager::Add(graphicsObj, name, this, Type::Light);
	pObj->Set(sceneLight, pLight);
	return pObj;
}

GameObj_Light* GameObj_Root::Attach(GraphicsObject* graphicsObj, SceneLightObject* sceneLight, PhongADS_Spot* pLight, const char* const name)
{
	GameObj_Light* pObj = (GameObj_Light*)GameObjectManager::Add(graphicsObj, name, this, Type::Light);
	pObj->Set(sceneLight, pLight);
	return pObj;
}
