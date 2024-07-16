#include "GraphicsObject.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "GameObject.h"

GraphicsObject::GraphicsObject(ModelNode* model, ShaderNode* shader, MaterialObject* material, TextureNode* text)
	: pModel(model), pShader(shader), pMaterial(material), pTexture(text)
{
}

void GraphicsObject::Render(ID3D11DeviceContext* context, GameObject* pGameObject)
{
	assert(context);
	assert(pGameObject);
	assert(this->pShader);

	ShaderManager::SetToCurrent(this->pShader);
	pShader->UpdateData(pGameObject, pMaterial);

	TextureManager::SetToCurrent(pTexture, context);
	if (this->pModel)
	{
		ModelManager::SetToCurrent(this->pModel, context);
		pModel->Render(context);
	}
}
