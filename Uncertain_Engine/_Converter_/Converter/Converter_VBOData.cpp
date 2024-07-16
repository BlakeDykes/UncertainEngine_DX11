#include "Converter_VBOData.h"
#include "tiny_gltf.h"

VBOData::VBO_TARGET Converter_VBOData::GetTarget(int GLTF_TINY_VAL)
{
	VBOData::VBO_TARGET VBOTarget;

	switch (GLTF_TINY_VAL)
	{
	case TINYGLTF_TARGET_ARRAY_BUFFER:
		VBOTarget = VBOData::VBO_TARGET::ARRAY_BUFFER;
		break;

	case TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER:
		VBOTarget = VBOData::VBO_TARGET::ELEMENT_ARRAY_BUFFER;
		break;

	default:
		VBOTarget = VBOData::VBO_TARGET::DEFAULT;
		break;
	}

	return VBOTarget;
}

VBOData::VBO_COMPONENT Converter_VBOData::GetComponent(int GLTF_TINY_VAL)
{
	VBOData::VBO_COMPONENT VBOComponent;

	switch (GLTF_TINY_VAL)
	{

	case TINYGLTF_COMPONENT_TYPE_BYTE:
		VBOComponent = VBOData::VBO_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		VBOComponent = VBOData::VBO_COMPONENT::BYTE;
		break;

	case TINYGLTF_COMPONENT_TYPE_SHORT:
		VBOComponent = VBOData::VBO_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		VBOComponent = VBOData::VBO_COMPONENT::UNSIGNED_SHORT;
		break;

	case TINYGLTF_COMPONENT_TYPE_INT:
		VBOComponent = VBOData::VBO_COMPONENT::INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		VBOComponent = VBOData::VBO_COMPONENT::UNSIGNED_INT;
		break;

	case TINYGLTF_COMPONENT_TYPE_FLOAT:
		VBOComponent = VBOData::VBO_COMPONENT::FLOAT;
		break;

	case TINYGLTF_COMPONENT_TYPE_DOUBLE:
		VBOComponent = VBOData::VBO_COMPONENT::DOUBLE;
		break;

	default:
		assert(false);
		VBOComponent = VBOData::VBO_COMPONENT::DEFAULT;
		break;

	}

	return VBOComponent;
}

VBOData::VBO_TYPE Converter_VBOData::GetType(int GLTF_TINY_VAL)
{
	VBOData::VBO_TYPE VBOType;

	switch (GLTF_TINY_VAL)
	{
	case TINYGLTF_TYPE_VEC2:
		VBOType = VBOData::VBO_TYPE::VEC2;
		break;

	case TINYGLTF_TYPE_VEC3:
		VBOType = VBOData::VBO_TYPE::VEC3;
		break;

	case TINYGLTF_TYPE_VEC4:
		VBOType = VBOData::VBO_TYPE::VEC4;
		break;

	case TINYGLTF_TYPE_MAT2:
		VBOType = VBOData::VBO_TYPE::MAT2;
		break;

	case TINYGLTF_TYPE_MAT3:
		VBOType = VBOData::VBO_TYPE::MAT3;
		break;

	case TINYGLTF_TYPE_MAT4:
		VBOType = VBOData::VBO_TYPE::MAT4;
		break;

	case TINYGLTF_TYPE_SCALAR:
		VBOType = VBOData::VBO_TYPE::SCALAR;
		break;

	case TINYGLTF_TYPE_VECTOR:
		VBOType = VBOData::VBO_TYPE::VECTOR;
		break;

	case TINYGLTF_TYPE_MATRIX:
		VBOType = VBOData::VBO_TYPE::MATRIX;
		break;

	default:
		assert(false);
		VBOType = VBOData::VBO_TYPE::DEFAULT;
		break;
	}

	return VBOType;
}

unsigned int Converter_VBOData::GetStride(VBOData::VBO_TYPE type)
{

	switch (type)
	{
	case VBOData::VBO_TYPE::VEC2:		return 2;
	case VBOData::VBO_TYPE::VEC3:		return 3;
	case VBOData::VBO_TYPE::VEC4:		return 4;
	case VBOData::VBO_TYPE::MAT2:		return 4;
	case VBOData::VBO_TYPE::MAT3:		return 9;
	case VBOData::VBO_TYPE::MAT4:		return 16;
	case VBOData::VBO_TYPE::SCALAR:		return 1;
	case VBOData::VBO_TYPE::VECTOR:		return 3;
	case VBOData::VBO_TYPE::MATRIX:		return 16;
	default:							return 0;
	}

}
