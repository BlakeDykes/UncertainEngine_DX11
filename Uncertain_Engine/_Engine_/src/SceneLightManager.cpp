#include "SceneLightManager.h"
#include "SceneLightObject.h"
#include "ShaderManager.h"
#include "GraphicsObject.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "GameObject.h"
#include "DLinkIterator.h"
#include "DLinkManager.h"

void SceneLightManager::Create(ID3D11Device*& device)
{
	assert(Instance().pDevice == nullptr);
	Instance().pDevice = device;

}

SceneLightObject* SceneLightManager::CreateSceneLightObj(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount, LightSourceStatus status)
{
	SceneLightManager* inst = &Instance();

	SceneLightObject* pSL = (SceneLightObject*)inst->BaseAdd();
	pSL->Set(dirCount, pointCount, spotCount);

	if ((bool)status)
	{
		inst->pActive = pSL;
	}

	return pSL;
}

SceneLightObject* SceneLightManager::CreateSceneLightObj(ShaderName shaderName, LightSourceStatus status)
{
	SceneLightManager* inst = &Instance();

	SceneLightObject* pSL = (SceneLightObject*)inst->BaseAdd();
	pSL->Set(ShaderManager::GetMaxLightCount(shaderName));

	if ((bool)status)
	{
		inst->pActive = pSL;
	}

	return pSL;
}

void SceneLightManager::SetStatus(SceneLightObject* pLS, LightSourceStatus status)
{
	SceneLightManager* inst = &Instance();
	
	if (pLS && pLS != inst->pActive && (bool)status)
	{
		inst->pActive = pLS;
	}
}

void SceneLightManager::DestroyAll()
{
	Instance().BaseClear();
}

void SceneLightManager::Update(_Time frameTime)
{
	ALIGN_UNUSED_VAR(frameTime);

	DLinkIterator* pIt = (DLinkIterator*)Instance().BaseGetIterator();
	SceneLightObject* pLight = (SceneLightObject*)pIt->Current();

	while(!pIt->IsDone())
	{
		pLight->Update();
		pLight = (SceneLightObject*)pIt->Next();
	}
}

SceneLightObject* SceneLightManager::GetActiveLightObj()
{
	return Instance().pActive;
}

UINT SceneLightManager::GetActiveDataSize()
{
	return Instance().pActive->GetMaxSize();
}

SceneLightManager::SceneLightManager()
	: ManagerBase(*new DLinkManager(), *new DLinkManager()), pDevice(nullptr), pActive(nullptr)
{
	this->SetReserve(INITIAL_RESERVED, DELTA_GROW);
}

SceneLightManager& SceneLightManager::Instance()
{
	static SceneLightManager instance;
	return instance;
}

NodeBase* SceneLightManager::CreateNode()
{
	return (NodeBase*)new SceneLightObject();
}
