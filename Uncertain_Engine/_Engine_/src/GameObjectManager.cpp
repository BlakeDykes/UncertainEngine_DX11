#include "GameObjectManager.h"
#include "_GameObjectWrapper.h"
#include "CommandQueueManager.h"
#include "PCSTreeReverseIterator.h"
#include "PCSNode.h"
#include "CommandQueue.h"
#include "Camera.h"
#include "Command.h"
#include "ReflectionManager.h"

GameObjectManager* GameObjectManager::instance = nullptr;

void GameObjectManager::Create()
{
	assert(!instance);

	if (!instance)
	{
		instance = new GameObjectManager();
	}
}

void GameObjectManager::Destroy()
{
	GameObjectManager* inst = GameObjectManager::GetInstance();

	delete inst;

	GameObjectManager::instance = nullptr;
}

void GameObjectManager::Update()
{
	GameObjectManager* inst = GameObjectManager::GetInstance();

	// Handle command queue
	Command* pCommand;

	while (inst->pCommandQueue->PopFront(pCommand) == true)
	{
		pCommand->Execute();
	}

	// Default tree
	PCSTreeForwardIterator* pIt = inst->poRootTree->GetForwardIT();
	GameObject* pCur = (GameObject*)pIt->CurrentItem();
	while (!pIt->IsDone())
	{
		pCur->Update();
		pCur = (GameObject*)pIt->Next();
	}

	for (auto it = inst->poReflected.begin(); it != inst->poReflected.end(); it++)
	{
		pIt = it->second->GetForwardIT();
		pCur = (GameObject*)pIt->CurrentItem();
		while (!pIt->IsDone())
		{
			pCur->Update();
			pCur = (GameObject*)pIt->Next();
		}
	}

	for (auto it = inst->poDeferred.begin(); it != inst->poDeferred.end(); it++)
	{
		pIt = it->second->GetForwardIT();
		pCur = (GameObject*)pIt->CurrentItem();
		while (!pIt->IsDone())
		{
			pCur->Update();
			pCur = (GameObject*)pIt->Next();
		}
	}

}

void GameObjectManager::Draw(ID3D11DeviceContext* context)
{
	GameObjectManager* inst = GameObjectManager::GetInstance();

	PCSTreeForwardIterator* pIt = inst->poRootTree->GetForwardIT();
	GameObject* pCur = (GameObject*)pIt->CurrentItem();

	while (!pIt->IsDone())
	{
		pCur->Render(context);
		pCur = (GameObject*)pIt->Next();
	}

	for (auto it = inst->poReflected.begin(); it != inst->poReflected.end(); it++)
	{
		pIt = it->second->GetForwardIT();
		pCur = (GameObject*)pIt->First();

		while (!pIt->IsDone())
		{
			pCur->Render(context);
			pCur = (GameObject*)pIt->Next();
		}
	}
}

void GameObjectManager::Draw_Reflections(ID3D11DeviceContext* context)
{
	GameObjectManager* inst = GameObjectManager::GetInstance();

	for (auto it = inst->poReflected.begin(); it != inst->poReflected.end(); it++)
	{
		ShaderManager::SetReflectedLighting(it->first->GetReflectionMat());

		PCSTreeForwardIterator* pIt = it->second->GetForwardIT();
		GameObject* pObj = (GameObject*)pIt->First();
		const Mat4& reflectionMat = it->first->GetReflectionMat();

		while (!pIt->IsDone())
		{
			pObj->ReflectWorld(reflectionMat);
			pObj->Render(context);
			pObj = (GameObject*)pIt->Next();
		}
	}
}

void GameObjectManager::Draw_Deferred(ID3D11DeviceContext* context)
{
	GameObjectManager* inst = GameObjectManager::GetInstance();
	ShaderManager::SetLighting();

	for (auto it = inst->poDeferred.begin(); it != inst->poDeferred.end(); it++)
	{
		PCSTreeForwardIterator* pIt = it->second->GetForwardIT();
		GameObject* pObj = (GameObject*)pIt->First();
		while (!pIt->IsDone())
		{
			pObj->Render(context);
			pObj = (GameObject*)pIt->Next();
		}
	}
}

GameObject* GameObjectManager::Add(GraphicsObject* graphicsObj, const char* const name, GameObject* pParent, GameObject::Type objectType)
{
	GameObjectManager* inst = GameObjectManager::GetInstance();
	GameObject* pObj = inst->CreateGameObject(graphicsObj, name, objectType, GameObject::RenderOrder::Default);

	inst->poRootTree->Insert(pObj, pParent ? pParent : inst->poRootTree->GetRoot());

	return pObj;
}

GameObject* GameObjectManager::Add_Reflected(ReflectionPlane* pReflection, GraphicsObject* graphicsObj, const char* const name, GameObject* pParent, GameObject::Type objectType)
{
	GameObjectManager* inst = GameObjectManager::GetInstance();
	GameObject* pObj = inst->CreateGameObject(graphicsObj, name, objectType, GameObject::RenderOrder::Reflected);
	pObj->SetReflectionParent(pReflection);

	if (inst->poReflected.count(pReflection) == 0)
	{
		inst->InitTree(inst->poReflected, pReflection, GameObject::RenderOrder::Reflected);
	}

	if (inst->FindTree(pParent) == inst->poReflected[pReflection])
	{
		inst->poReflected[pReflection]->Insert(pObj, pParent);
	}
	else
	{
		inst->poReflected[pReflection]->Insert(pObj, inst->poReflected[pReflection]->GetRoot());
	}

	return pObj;
}

GameObject* GameObjectManager::Add_Deferred(ReflectionPlane* pReflection, GraphicsObject* graphicsObj, const char* const name, GameObject* pParent, GameObject::Type objectType)
{
	GameObjectManager* inst = GameObjectManager::GetInstance();
	GameObject* pObj = inst->CreateGameObject(graphicsObj, name, objectType, GameObject::RenderOrder::Deferred);
	pObj->SetReflectionParent(pReflection);

	if (inst->poDeferred.count(pReflection) == 0)
	{
		inst->InitTree(inst->poDeferred, pReflection, GameObject::RenderOrder::Deferred);
	}

	if (inst->FindTree(pParent) == inst->poDeferred[pReflection])
	{
		inst->poDeferred[pReflection]->Insert(pObj, pParent);
	}
	else
	{
		inst->poDeferred[pReflection]->Insert(pObj, inst->poDeferred[pReflection]->GetRoot());
	}

	return pObj;
}

GameObject* GameObjectManager::CreateReflectionObject(GraphicsObject* graphicsObj, const Vec3& scale, const Quat& rot, const Vec3& trans, const char* const name)
{
	return new GameObject(graphicsObj, name, scale, rot, trans, GameObject::RenderOrder::ReflectionObject);
}

void GameObjectManager::Remove(GameObject& pNode)
{
	switch (pNode.GetRenderOrder())
	{
	case GameObject::RenderOrder::Default:			GetInstance()->poRootTree->Remove(&pNode);
	case GameObject::RenderOrder::Reflected:		GetInstance()->poReflected[pNode.GetReflectionParent()]->Remove(&pNode);
	case GameObject::RenderOrder::Deferred:			GetInstance()->poDeferred[pNode.GetReflectionParent()]->Remove(&pNode);
	case GameObject::RenderOrder::ReflectionObject: GetInstance()->RemoveReflectionObject(pNode);
	default:
		break;
	}
}

GameObject* GameObjectManager::CreateGameObject(GraphicsObject* graphicsObj, const char* const name, GameObject::Type objectType, GameObject::RenderOrder order)
{
	switch (objectType)
	{
	case GameObject::Type::Default:		return new GameObject(graphicsObj, name, order);
	case GameObject::Type::Root:		return new GameObj_Root(name, order);
	case GameObject::Type::Light:		return new GameObj_Light(graphicsObj, name, order);
	default:
		assert(false);
		return nullptr;
	}
}

void GameObjectManager::RemoveReflectionObject(GameObject& node)
{
	ReflectionPlane* pReflection = node.GetReflectionParent();

	if (poReflected.count(pReflection) > 0)
	{
		delete poReflected[pReflection];
		poReflected.erase(pReflection);
	}

	if (poDeferred.count(pReflection) > 0)
	{
		PCSTreeForwardIterator* pIt = poDeferred[pReflection]->GetForwardIT();
		GameObject* pCur = (GameObject*)pIt->First();

		while (!pIt->IsDone())
		{
			pCur->SetDefaultRenderOrder();
		}

		GameObj_Root* pRoot = (GameObj_Root*)poDeferred[pReflection]->GetRoot();
		pRoot->SetName("Ex-Deferred Root");

		this->poRootTree->Insert(pRoot, nullptr);
		poDeferred.erase(pReflection);
	}
}

GameObj_Root* GameObjectManager::InitTree(std::map<ReflectionPlane*, PCSTree*>& map, ReflectionPlane* pPlane, GameObject::RenderOrder rOrder)
{
	GameObj_Root* pDeferredRoot = new GameObj_Root("Default Root", rOrder);
	assert(pDeferredRoot);

	map[pPlane] = new PCSTree();

	map[pPlane]->Insert(pDeferredRoot, nullptr);

	return pDeferredRoot;
}

PCSTree* GameObjectManager::FindTree(GameObject* pNode)
{
	if (pNode == nullptr) return nullptr;

	switch (pNode->GetRenderOrder())
	{
	case GameObject::RenderOrder::Default:			return poRootTree;
	case GameObject::RenderOrder::Deferred: 
	{
		if (poDeferred.count(pNode->GetReflectionParent()) > 0)
		{
			return poDeferred[pNode->GetReflectionParent()];
		}
		else
		{
			return nullptr;
		}
	}
	case GameObject::RenderOrder::Reflected:
	{
		if (poReflected.count(pNode->GetReflectionParent()) > 0)
		{
			return poReflected[pNode->GetReflectionParent()];
		}
		else
		{
			return nullptr;
		}
	}
	case GameObject::RenderOrder::ReflectionObject:
	default:
		return nullptr;
	}
}

GameObjectManager::GameObjectManager()
	: poReflected(), poDeferred()
{
	GameObj_Root* pDefaultRoot = new GameObj_Root("Default Root", GameObject::RenderOrder::Default);

	this->poRootTree = new PCSTree();
	this->poRootTree->Insert(pDefaultRoot, nullptr);

	this->pCommandQueue = CommandQueueManager::Add(ComQueueName::GAME);
}

GameObjectManager* GameObjectManager::GetInstance()
{
	assert(instance);
	return instance;
}

GameObjectManager::~GameObjectManager()
{
	delete this->poRootTree;

	auto it = poDeferred.begin();
	while (it != poDeferred.end())
	{
		delete it->second;
		it = poDeferred.erase(it);
	}

	it = poReflected.begin();
	while (it != poReflected.end())
	{
		delete it->second;
		it = poReflected.erase(it);
	}
}
