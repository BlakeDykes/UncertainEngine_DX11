#pragma once

#include "DLink.h"
#include "ShaderObject.h"
#include "SO_NULL.h"

class ShaderNode : public DLink
{

public:
	ShaderNode();
	ShaderNode(const ShaderNode&) = delete;
	ShaderNode& operator=(const ShaderNode&) = delete;
	virtual ~ShaderNode();

	void UpdateData(const GameObject* pGO, const MaterialObject* pMaterial) { pShader->UpdateData(pGO, pMaterial); }

	void Set(ShaderName name, ShaderObject* shader = nullptr);

	ShaderName GetShaderName() { return this->Name; }

	bool GetShaderObj(ShaderObject*& pShaderOut);
	void SetToCurrent();

	// Inherited via DLink
	void Wash() override;
	bool Compare(NodeBase& pNode) override;
	const char* GetName() override;
	void Print() override;

private:
	ShaderName Name;
	ShaderObject* pShader;
};