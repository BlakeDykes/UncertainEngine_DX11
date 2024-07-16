#pragma once

#include "DLink.h"
#include <d3d11.h>
#include "Model.h"

class ModelNode : public DLink
{

public:
	ModelNode();
	ModelNode(const ModelNode&) = delete;
	ModelNode& operator=(const ModelNode&) = delete;
	virtual ~ModelNode();

	void Set(Model& model) { poModel = &model; }
	void SetNull();

	void SetToCurrent(ID3D11DeviceContext* context) { poModel->SetToCurrent(context); }
	void Render(ID3D11DeviceContext* context) { poModel->Render(context); }
	void RenderMesh(ID3D11DeviceContext* context, int meshnum) { poModel->RenderMesh(context, meshnum); }

	int GetMeshCount() { return poModel->GetMeshCount(); }
	bool ValidMeshNum(int i) { return poModel->ValidMeshNum(i); }

	void SetType(Model::ModelType mType) const { poModel->MType = mType; }
	Model::ModelType GetType() const { return poModel ? poModel->MType : Model::ModelType::Uninitialized; }

	// Inherited via DLink
	void Wash() override { delete poModel; poModel = nullptr; }
	bool Compare(NodeBase& pNode) override;

private:
	Model* poModel;
};