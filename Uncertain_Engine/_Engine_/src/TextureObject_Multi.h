#pragma once

#include "TextureObject.h"
#include <vector>

class TextureObject_Multi : public TextureObject
{
public:
	TextureObject_Multi(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV);
	virtual ~TextureObject_Multi();

	void SetToCurrent(ID3D11DeviceContext* context) override;

	virtual void AddTextureObject(ID3D11Device* device, DirectX::ScratchImage* image) override;

private:
	std::vector<TextureObject*> Textures;
};