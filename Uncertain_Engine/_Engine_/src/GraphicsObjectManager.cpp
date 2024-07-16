#include "GraphicsObjectManager.h"
#include "ShaderNode.h"
#include "ModelNode.h"

void GraphicsObjectManager::DestroyAll()
{
	GraphicsObjectManager* inst = &Instance();

	for (auto it = inst->GraphicsObjects.begin(); it != inst->GraphicsObjects.end();)
	{
		delete it->second;
		inst->GraphicsObjects.erase(it++);
	}
}

GraphicsObject* GraphicsObjectManager::CreateGraphicsObj(ModelNode* model, ShaderNode* shader, MaterialObject* material, TextureNode* text)
{
	GraphicsObjectManager* inst = &Instance();

	MaterialObject* mat = material == nullptr ? &NULL_MAT : material;
	TextureNode* texture = text == nullptr ? &NULL_TEX : text;


	std::string hashString = CreateHashString(model, shader, mat, texture);

	auto node = inst->GraphicsObjects.find(hashString);

	if (node != inst->GraphicsObjects.end())
	{
		return node->second;
	}
	else
	{
		GraphicsObject* pObj = new GraphicsObject(model, shader, mat, texture);
		inst->GraphicsObjects.emplace(std::make_pair(hashString, pObj));

		return pObj;
	}
}

void GraphicsObjectManager::Remove(GraphicsObject& pGO)
{
	GraphicsObjectManager* inst = &Instance();

	std::string hashString = CreateHashString(pGO.GetModel(), pGO.GetShader(), pGO.GetMaterial(), pGO.GetTexture());

	auto node = inst->GraphicsObjects.find(hashString);
	if (node != inst->GraphicsObjects.end())
	{
		delete node->second;
		inst->GraphicsObjects.erase(node);
	}
}

std::string GraphicsObjectManager::CreateHashString(ModelNode* model, ShaderNode* shader, MaterialObject* material, TextureNode* text)
{
	return std::to_string(model->GetHandle())
		+ std::to_string(shader->GetHandle())
		+ std::to_string(material->GetHandle())
		+ std::to_string(text->GetHandle());
}

GraphicsObjectManager::GraphicsObjectManager()
	: GraphicsObjects()
{
}

GraphicsObjectManager& GraphicsObjectManager::Instance()
{
	static GraphicsObjectManager instance;
	return instance;
}
