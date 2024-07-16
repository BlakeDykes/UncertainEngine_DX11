#pragma once

#include <d3d11.h>
#include "d3dUtil.h"

class MaterialObject;
class TextureNode;
class ShaderNode;
class ModelNode;
class GameObject;

class GraphicsObject
{
protected:
	ModelNode* pModel;
	ShaderNode* pShader;
	MaterialObject* pMaterial;
	TextureNode* pTexture;

public:
	GraphicsObject(ModelNode* model, ShaderNode* shader, MaterialObject* material, TextureNode* text);
	virtual ~GraphicsObject() = default;

	ModelNode* GetModel() { return pModel; }
	ShaderNode* GetShader() { return pShader; }
	MaterialObject* GetMaterial() { return pMaterial; }
	TextureNode* GetTexture() { return pTexture; }

	virtual void Render(ID3D11DeviceContext* context, GameObject* pGameObject);

	void SetShader(ShaderNode* shader) { pShader = shader; }
	void SetTexture(TextureNode* texture) { pTexture = texture; }
	void SetMaterial(MaterialObject* material) { pMaterial = material; }
};