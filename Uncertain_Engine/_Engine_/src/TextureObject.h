#pragma once

#include <Windows.h>
#include <d3d11.h>
#include "DirectXTex.h"

class TextureObject
{
public:

	enum FileType
	{
		PNG,
		TGA
	};

	TextureObject();
	virtual ~TextureObject();
	TextureObject(const TextureObject&) = delete;
	TextureObject(TextureObject&&) = delete;
	TextureObject& operator=(const TextureObject&) & = delete;
	TextureObject& operator=(TextureObject&&) & = delete;

	TextureObject(ID3D11Device* device, DirectX::ScratchImage* textImage, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV);

	virtual void SetToCurrent(ID3D11DeviceContext* context);

	void SetSamplerToCurrent(ID3D11DeviceContext* context, int samplerSlot);
	void SetTexToCurrent(ID3D11DeviceContext* context, int texSlot);

	virtual void AddTextureObject(ID3D11Device* device, DirectX::ScratchImage* image) { AZUL_UNUSED_VAR(device); AZUL_UNUSED_VAR(image); }

protected:
	ID3D11ShaderResourceView* poTextureView;
	ID3D11SamplerState* poSampler;
	D3D11_FILTER Filter;
	D3D11_TEXTURE_ADDRESS_MODE AddressU;
	D3D11_TEXTURE_ADDRESS_MODE AddressV;

};

