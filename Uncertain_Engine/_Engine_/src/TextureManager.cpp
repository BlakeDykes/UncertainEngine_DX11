#include "TextureManager.h"
#include "TextureObject_Multi.h"
#include "DLinkManager.h"

void TextureManager::Create(ID3D11Device*& device)
{
	Instance().pDevice = device;
}

void TextureManager::DestroyAll()
{
	TextureManager* inst = &Instance();

	inst->BaseClear();
}

TextureNode* TextureManager::CreateTextureObj(LPCWSTR texFilePath, FileType fileType, D3D11_FILTER filter,
	D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV)
{
	TextureManager* inst = &Instance();

	DirectX::ScratchImage textImage = inst->LoadTextureFromFile(texFilePath, fileType);

	TextureNode* pTex = (TextureNode*)inst->BaseAdd();
	assert(pTex);
	
	pTex->Set(inst->pDevice, &textImage, filter, addressU, addressV);

	return pTex;
}

TextureNode* TextureManager::CreateTextureObj(std::vector<LPCWSTR>& texFilePath, FileType fileType, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV)
{
	TextureManager* inst = &Instance();

	TextureNode* pTextureObj = (TextureNode*)inst->BaseAdd();

	pTextureObj->SetMulti(inst->pDevice, filter, addressU, addressV);

	for (LPCWSTR fp : texFilePath)
	{
		DirectX::ScratchImage texImage = inst->LoadTextureFromFile(fp, fileType);
		pTextureObj->AddTextureObject(inst->pDevice, &texImage);
	}

	return pTextureObj;
}

void TextureManager::SetToCurrent(TextureNode* t, ID3D11DeviceContext* context)
{
	if (t == nullptr || !t->isValid()) return;

	TextureManager* inst = &Instance();
	UINT tHandle = t->GetHandle();

	if (tHandle != inst->mCurrentT)
	{
		inst->mCurrentT = tHandle;
		t->SetToCurrent(context);
	}
}

DirectX::ScratchImage TextureManager::LoadTextureFromFile(LPCWSTR texFilePath, FileType fileType)
{
	DirectX::ScratchImage textImage;
	HRESULT hr;

	switch (fileType)
	{
	case FileType::PROTO:
		assert(false);
		break;

	case FileType::TGA:
	{
		hr = LoadFromTGAFile(texFilePath, nullptr, textImage);
		assert(SUCCEEDED(hr));
		break;
	}

	case FileType::PNG:
	default:
	{
		hr = LoadFromWICFile(texFilePath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, textImage);
		assert(SUCCEEDED(hr));
		break;
	}
	}

	return textImage;
}

TextureManager::TextureManager()
	: ManagerBase(*new DLinkManager(), *new DLinkManager()), pDevice(nullptr), mCurrentT(0)
{
	this->SetReserve(INITIAL_RESERVED, DELTA_GROW);
}

TextureManager& TextureManager::Instance()
{
	static TextureManager inst;
	return inst;
}

NodeBase* TextureManager::CreateNode()
{
	return (NodeBase*)new TextureNode();
}
