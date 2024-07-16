#include "TextureNode.h"

TextureNode::TextureNode()
	: poTex(nullptr)
{
}

TextureNode::~TextureNode()
{
	delete poTex;
}

void TextureNode::Wash()
{
	delete poTex;
	this->poTex = nullptr;
}

bool TextureNode::Compare(NodeBase& pNode)
{
	TextureNode* pComp = (TextureNode*)& pNode;

	return pComp->poTex == pComp->poTex;
}
