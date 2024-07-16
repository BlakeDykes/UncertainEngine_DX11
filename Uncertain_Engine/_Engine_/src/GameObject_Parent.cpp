#include "GameObject_Parent.h"

GameObject_Parent::GameObject_Parent(SceneLightObject* sceneLight)
	: GameObject(nullptr), GameObjects(), Lights(), pSceneLight(sceneLight)
{
}

GameObject_Parent::~GameObject_Parent()
{
	for (GameObject* pGO : GameObjects)
	{
		delete pGO;
	}
	for (LightWorldUpdater* pLight : Lights)
	{
		delete pLight;
	}
}

GameObject* GameObject_Parent::AddGameObject(GraphicsObject*& go)
{
	GameObject* pGO = new GameObject(go);
	GameObjects.push_back(pGO);

	return pGO;
}

void GameObject_Parent::AddLight(LightType lType, size_t index)
{
	assert(pSceneLight);

	switch (lType)
	{
	case Directional:	Lights.push_back(new DirUpdater(pSceneLight, index));  break;
	case Point:			Lights.push_back(new PointUpdater(pSceneLight, index));  break;
	case Spot:			Lights.push_back(new SpotUpdater(pSceneLight, index));  break;
	default:
		assert(false);
		break;
	}
}

void GameObject_Parent::AddLight(PhongADS_Directional& light)
{
	Lights.push_back(new DirUpdater(pSceneLight, pSceneLight->GetIndex(&light)));
}

void GameObject_Parent::AddLight(PhongADS_Point& light)
{
	Lights.push_back(new PointUpdater(pSceneLight, pSceneLight->GetIndex(&light)));
}

void GameObject_Parent::AddLight(PhongADS_Spot& light)
{
	Lights.push_back(new SpotUpdater(pSceneLight, pSceneLight->GetIndex(&light)));
}

void GameObject_Parent::Update()
{
	GameObject::Update();

	for (GameObject* pGO : GameObjects)
	{
		pGO->Update(this->poWorld);
	}
	for (LightWorldUpdater* pLight : Lights)
	{
		pLight->SendLightWorldUpdate(*this->poWorld);
	}

}

void GameObject_Parent::Render(ID3D11DeviceContext* context)
{
	for (GameObject* pGO : GameObjects)
	{
		pGO->Render(context);
	}
}

void GameObject_Parent::ReflectedRender(ID3D11DeviceContext* context, Matrix* reflectionMatrix)
{
	for (GameObject* pGO : GameObjects)
	{
		pGO->ReflectedRender(context, reflectionMatrix);
	}
}
