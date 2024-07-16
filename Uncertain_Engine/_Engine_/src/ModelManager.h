#pragma once

#include "ManagerBase.h"
#include "ModelNode.h"
#include "_ModelWrapper.h"
#include <vector>

class ModelManager : public ManagerBase
{
	static const int INITIAL_RESERVED = 10;
	static const int DELTA_GROW = 10;

public:
	virtual ~ModelManager();

	static void Create(ID3D11Device*& device);
	static void DestroyAll();

	static ModelNode* CreateModel(StandardVertex* pVerts, int nverts, TriangleByIndex* ptlist, int ntri);
	static ModelNode* CreateModel(const char* const modelFilePath);
	static ModelNode* CreateModel(Model::ModelType pm, float scale = 1.0f);
	static ModelNode* FindModel(Model::ModelType type);

	static ModelNode* CreateSphere(int vslice = 15, int hslice = 15, float scale = 1.0f);
	static ModelNode* CreateFlatPlane(float len, float urep, float vrep);
	static ModelNode* CreateSkybox(float scale, SkyboxTextures texName);
	static ModelNode* CreateTerrain(LPCWSTR heightMapFP,
		float scale, float maxHeight, float yTrans, int uRep, int vRep);
	static ModelNode* CreateAudioSpectrum(UINT sampleCount, float len, float yBase);

	static void SetToCurrent(ModelNode*& m, ID3D11DeviceContext* context);

private:
	ModelNode* poComp;
	ID3D11Device* pDevice;
	ModelHandle mCurrentModel;

	ModelManager();
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator =(const ModelManager&) = delete;

	static ModelManager* instance;
	static ModelManager& Instance();

	// Inherited via ManagerBase
	NodeBase* CreateNode() override;
};