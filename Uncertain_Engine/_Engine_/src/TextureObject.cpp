#include "TextureObject.h"
#include "d3dUtil.h"

TextureObject::TextureObject()
	: poTextureView(nullptr), poSampler(nullptr), 
	Filter(D3D11_FILTER_MIN_MAG_MIP_LINEAR), AddressU(D3D11_TEXTURE_ADDRESS_WRAP), AddressV(D3D11_TEXTURE_ADDRESS_WRAP)
{
}

TextureObject::TextureObject(ID3D11Device* device, DirectX::ScratchImage* textImage, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV)
{
	CreateShaderResourceView(device, textImage->GetImage(0, 0, 0), textImage->GetImageCount(), textImage->GetMetadata(), &poTextureView);

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

TextureObject::~TextureObject()
{
	if (poTextureView != nullptr)
	{
		ReleaseAndDeleteCOMobject(poTextureView);
		poTextureView = nullptr;
	}
	if (poSampler != nullptr)
	{
		ReleaseAndDeleteCOMobject(poSampler);
		poSampler = nullptr;
	}
}

void TextureObject::SetToCurrent(ID3D11DeviceContext* context)
{
	SetTexToCurrent(context, 0);
	SetSamplerToCurrent(context, 0);
}

void TextureObject::SetSamplerToCurrent(ID3D11DeviceContext* context, int samplerSlot)
{
	context->PSSetSamplers(samplerSlot, 1, &poSampler);
}

void TextureObject::SetTexToCurrent(ID3D11DeviceContext* context, int texSlot)
{
	context->PSSetShaderResources(texSlot, 1, &poTextureView);
}
