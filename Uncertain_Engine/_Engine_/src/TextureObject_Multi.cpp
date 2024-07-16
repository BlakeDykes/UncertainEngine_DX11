#include "TextureObject_Multi.h"

TextureObject_Multi::TextureObject_Multi(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV)
	: TextureObject()
{
	this->Filter = filter;
	this->AddressU = addressU;
	this->AddressV = addressV;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = filter;
	sampDesc.AddressU = addressU;
	sampDesc.AddressV = addressV;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRESULT hr = device->CreateSamplerState(&sampDesc, &poSampler);
	assert(SUCCEEDED(hr));
}


TextureObject_Multi::~TextureObject_Multi()
{
	for (TextureObject* t : Textures)
	{
		delete t;
	}
}

void TextureObject_Multi::SetToCurrent(ID3D11DeviceContext* context)
{
	SetSamplerToCurrent(context, 0);

	int i = 0;
	for (TextureObject* t : Textures)
	{
		t->SetTexToCurrent(context, i++);
	}
}

void TextureObject_Multi::AddTextureObject(ID3D11Device* device, DirectX::ScratchImage* image)
{
	TextureObject* pText = new TextureObject(device, image, this->Filter, this->AddressU, this->AddressV);
	Textures.push_back(pText);
}
