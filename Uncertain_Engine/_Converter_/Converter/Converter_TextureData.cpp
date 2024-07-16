#include "Converter_TextureData.h"
#include "tiny_gltf.h"

TextureData::TEXTURE_COMPONENT Converter_TextureData::GetComponent(int GLTF_TINY_VAL)
{
	TextureData::TEXTURE_COMPONENT vboComponent;

	switch (GLTF_TINY_VAL)
	{

	case TINYGLTF_COMPONENT_TYPE_BYTE:
		vboComponent = TextureData::TEXTURE_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		vboComponent = TextureData::TEXTURE_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_SHORT:
		vboComponent = TextureData::TEXTURE_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		vboComponent = TextureData::TEXTURE_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_INT:
		vboComponent = TextureData::TEXTURE_COMPONENT::INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		vboComponent = TextureData::TEXTURE_COMPONENT::UNSIGNED_INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_FLOAT:
		vboComponent = TextureData::TEXTURE_COMPONENT::FLOAT;
		break;

	case TINYGLTF_COMPONENT_TYPE_DOUBLE:
		vboComponent = TextureData::TEXTURE_COMPONENT::DOUBLE;
		break;

	default:
		assert(false);
		vboComponent = TextureData::TEXTURE_COMPONENT::DEFAULT;
		break;

	}

	return vboComponent;
}