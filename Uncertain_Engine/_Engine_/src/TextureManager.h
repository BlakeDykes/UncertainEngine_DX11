#pragma once

#include "ManagerBase.h"
#include "DirectXTex.h"
#include "TextureNode.h"
#include <vector>

enum class FileType
{
	PNG,
	TGA,
	PROTO
};

class TextureData;

class TextureManager : public ManagerBase
{
	static const int INITIAL_RESERVED = 10;
	static const int DELTA_GROW = 10;

public:
	static void Create(ID3D11Device*& device);
	static void DestroyAll();

	static TextureNode * CreateTextureObj(LPCWSTR texFilePath, FileType fileType, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP);

	static TextureNode* CreateTextureObj(std::vector<LPCWSTR>& texFilePath, FileType fileType, D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_MODE addressU = D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_MODE addressV = D3D11_TEXTURE_ADDRESS_WRAP);

	static void SetToCurrent(TextureNode* t, ID3D11DeviceContext* context);

private:

	DirectX::ScratchImage LoadTextureFromFile(LPCWSTR texFilePath, FileType fileType);
	TextureManager();
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator =(const TextureManager&) = delete;

	ID3D11Device* pDevice;

	NodeBase::Handle mCurrentT;

	static TextureManager& Instance();

	// Inherited via ManagerBase
	NodeBase* CreateNode() override;
};