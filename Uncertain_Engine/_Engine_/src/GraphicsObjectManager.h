#pragma once

#include <unordered_map>
#include <string>

#include "TextureNode_NULL.h"
#include "MaterialObject_NULL.h"
#include "GraphicsObject.h"

class ShaderNode;
class ModelNode;

static TextureNode NULL_TEX;
static MaterialObject_NULL NULL_MAT;

class GraphicsObjectManager
{
public:
	static void DestroyAll();

	static GraphicsObject* CreateGraphicsObj(ModelNode* model, ShaderNode* shader,
		MaterialObject* material = (MaterialObject*)&NULL_MAT, TextureNode* text = (TextureNode_NULL*)&NULL_TEX);

	static void Remove(GraphicsObject& pGO);

private:
	std::unordered_map<std::string, GraphicsObject*> GraphicsObjects;

	static std::string CreateHashString(ModelNode* model, ShaderNode* shader, MaterialObject* material, TextureNode* text);

	GraphicsObjectManager();
	GraphicsObjectManager(const GraphicsObjectManager&) = delete;
	GraphicsObjectManager& operator=(const GraphicsObjectManager&) = delete;

	static GraphicsObjectManager& Instance();
};
