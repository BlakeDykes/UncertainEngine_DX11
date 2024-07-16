#pragma once

#include "TextureObject.h"

class TextureObject_NULL : public TextureObject
{
public:
	void SetToCurrent(ID3D11DeviceContext* context) override { ALIGN_UNUSED_VAR(context); }
};
