#pragma once

#include "TextureNode.h"

class TextureNode_NULL : public TextureNode
{
public:
	void SetToCurrent(ID3D11DeviceContext* context) override { ALIGN_UNUSED_VAR(context); }
};
