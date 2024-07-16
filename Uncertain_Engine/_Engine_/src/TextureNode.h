#pragma once

#include "DLink.h"
#include <d3d11.h>
#include "TextureObject.h"
#include "TextureObject_Multi.h"

class TextureNode : public DLink
{

public:
	TextureNode();
	TextureNode(const TextureNode&) = delete;
	TextureNode& operator=(const TextureNode&) = delete;
	virtual ~TextureNode();

	void Set(TextureObject& tex) { assert(!poTex); this->poTex = &tex; }
	virtual void SetToCurrent(ID3D11DeviceContext* context) { assert(poTex); this->poTex->SetToCurrent(context); }
	
	void Set(ID3D11Device* device, DirectX::ScratchImage* textImage, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV) { assert(!poTex); poTex = new TextureObject(device, textImage, filter, addressU, addressV); }
	void SetMulti(ID3D11Device* device, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressU, D3D11_TEXTURE_ADDRESS_MODE addressV) { assert(!poTex); poTex = new TextureObject_Multi(device, filter, addressU, addressV); }
	void AddTextureObject(ID3D11Device* device, DirectX::ScratchImage* image) { assert(poTex); poTex->AddTextureObject(device, image); }

	bool isValid() { return poTex != nullptr && IsHandleValid(); }

	// Inherited via DLink
	void Wash() override;
	bool Compare(NodeBase& pNode) override;

private:
	TextureObject* poTex;
};