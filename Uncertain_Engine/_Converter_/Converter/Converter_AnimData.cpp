#include "Converter_AnimData.h"
#include "tiny_gltf.h"

AnimData::OUTPUT_TYPE Converter_AnimData::GetOutputType(int GLTF_TINY_VAL)
{
	AnimData::OUTPUT_TYPE type;

	switch (GLTF_TINY_VAL)
	{
	case TINYGLTF_TYPE_SCALAR:
		type = AnimData::OUTPUT_TYPE::SCALAR;
		break;
	case TINYGLTF_TYPE_VEC2:
		type = AnimData::OUTPUT_TYPE::VEC2;
		break;
	case TINYGLTF_TYPE_VEC3:
		type = AnimData::OUTPUT_TYPE::VEC3;
		break;
	case TINYGLTF_TYPE_VEC4:
		type = AnimData::OUTPUT_TYPE::VEC4;
		break;

	case TINYGLTF_TYPE_MAT2:
	case TINYGLTF_TYPE_MAT3:
	case TINYGLTF_TYPE_MAT4:
	case TINYGLTF_TYPE_VECTOR:
	case TINYGLTF_TYPE_MATRIX:
	default:
		assert(false);
		type = AnimData::OUTPUT_TYPE::SCALAR;
		break;
	}

	return type;
}

AnimData::PATH Converter_AnimData::GetPath(std::string gltfPath)
{

	if (gltfPath == "translation")
	{
		return AnimData::PATH::TRANSLATION;
	}

	else if (gltfPath == "rotation")
	{
		return AnimData::PATH::ROTATION;
	}

	else if (gltfPath == "scale")
	{
		return AnimData::PATH::SCALE;
	}
	else
	{
		assert(false);
		return AnimData::PATH::SCALE;
	}
}