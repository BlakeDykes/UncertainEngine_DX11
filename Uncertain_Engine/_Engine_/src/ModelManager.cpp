#include "ModelManager.h"
#include "SO_ColorSelected3D.h"
#include "SO_SimpleColor3D.h"
#include "DLinkManager.h"

ModelManager* ModelManager::instance = nullptr;

ModelManager::~ModelManager()
{
	delete this->poComp;
	BaseClear();
}

void ModelManager::Create(ID3D11Device*& device)
{
	instance = new ModelManager();
	instance->pDevice = device;
}

void ModelManager::DestroyAll()
{
	delete instance;
}

ModelNode* ModelManager::CreateModel(StandardVertex* pVerts, int nverts, TriangleByIndex* ptlist, int ntri)
{
	ModelManager* inst = &Instance();

	Model* pModel = new Model(inst->pDevice, pVerts, nverts, ptlist, ntri);
	
	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::CreateModel(const char* const modelFilePath)
{
	ModelManager* inst = &Instance();
	
	Model* pModel = new Model(inst->pDevice, modelFilePath);
	
	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::CreateModel(Model::ModelType pm, float scale)
{
	ModelManager* inst = &Instance();

	Model* pModel = new Model(inst->pDevice, pm, scale);

	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::FindModel(Model::ModelType type)
{
	ModelManager* inst = &Instance();
	inst->poComp->SetType(type);

	return (ModelNode*)inst->BaseFind((NodeBase&)*inst->poComp);
}

ModelNode* ModelManager::CreateSphere(int vslice, int hslice, float scale)
{
	ModelManager* inst = &Instance();

	Model* pModel = new Model(inst->pDevice, vslice, hslice, scale);

	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::CreateFlatPlane(float len, float urep, float vrep)
{
	ModelManager* inst = &Instance();

	Model_FlatPlane* pModel = new Model_FlatPlane(inst->pDevice, len, urep, vrep);
	
	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::CreateSkybox(float scale, SkyboxTextures texName)
{
	ModelManager* inst = &Instance();

	Model_Skybox* pModel = new Model_Skybox(inst->pDevice, texName, scale);
	
	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::CreateTerrain(LPCWSTR heightMapFP, float scale, float maxHeight, float yTrans, int uRep, int vRep)
{
	ModelManager* inst = &Instance();

	Model_Terrain* pModel = new Model_Terrain(inst->pDevice, heightMapFP, scale, maxHeight, yTrans, uRep, vRep);
	
	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

ModelNode* ModelManager::CreateAudioSpectrum(UINT sampleCount, float len, float yBase)
{
	ModelManager* inst = &Instance();

	Model_AudioSpectrum* pModel = new Model_AudioSpectrum(inst->pDevice, sampleCount, len, yBase);
	
	ModelNode* pNode = (ModelNode*)inst->BaseAdd();
	pNode->Set(*pModel);

	return pNode;
}

void ModelManager::SetToCurrent(ModelNode*& m, ID3D11DeviceContext* context)
{
	ModelManager* inst = &Instance();
	UINT handle = m->GetHandle();

	if (handle != inst->mCurrentModel)
	{
		inst->mCurrentModel = handle;
		m->SetToCurrent(context);
	}
}

ModelManager::ModelManager()
	: ManagerBase(*new DLinkManager(), *new DLinkManager()), poComp(new ModelNode()), pDevice(nullptr), mCurrentModel(0)
{
	poComp->SetNull();
	this->SetReserve(INITIAL_RESERVED, DELTA_GROW);
}

ModelManager& ModelManager::Instance()
{
	assert(instance != nullptr);
	return *instance;
}

NodeBase* ModelManager::CreateNode()
{
	return (NodeBase*)new ModelNode();
}
