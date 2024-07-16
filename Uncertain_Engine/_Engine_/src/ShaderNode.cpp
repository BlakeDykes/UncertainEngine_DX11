#include "ShaderNode.h"

ShaderNode::ShaderNode()
	: Name(ShaderName::UNINITIALIZED), pShader(nullptr)
{
}

ShaderNode::~ShaderNode()
{
	delete pShader;
}

void ShaderNode::Set(ShaderName name, ShaderObject* shader)
{
	this->Name = name;
	this->pShader = shader;
}

bool ShaderNode::GetShaderObj(ShaderObject*& pShaderOut)
{
	pShaderOut = pShader;
	return pShader != nullptr;
}

void ShaderNode::SetToCurrent()
{
	if (pShader)
	{
		pShader->SetToCurrent();
	}
}

void ShaderNode::Wash()
{
	delete this->pShader;
	this->pShader = nullptr;
}

bool ShaderNode::Compare(NodeBase& pNode)
{
	ShaderNode* pComp = (ShaderNode*)&pNode;

	return Name == pComp->GetShaderName();
}

const char* ShaderNode::GetName()
{
	return Stringify(Name);
}

void ShaderNode::Print()
{
}
