#include "Converter_MeshData.h"

MeshData::RENDER_MODE Converter_MeshData::GetMode(int GLTF_TINY_VAL)
{
	MeshData::RENDER_MODE renderMode;

	switch (GLTF_TINY_VAL)
	{

	case TINYGLTF_MODE_POINTS:
		renderMode = MeshData::RENDER_MODE::MODE_POINTS;
		break;

	case TINYGLTF_MODE_LINE:
		renderMode = MeshData::RENDER_MODE::MODE_LINE;
		break;

	case TINYGLTF_MODE_LINE_LOOP:
		renderMode = MeshData::RENDER_MODE::MODE_LINE_LOOP;
		break;

	case TINYGLTF_MODE_LINE_STRIP:
		renderMode = MeshData::RENDER_MODE::MODE_LINE_STRIP;
		break;

	case TINYGLTF_MODE_TRIANGLES:
		renderMode = MeshData::RENDER_MODE::MODE_TRIANGLES;
		break;

	case TINYGLTF_MODE_TRIANGLE_STRIP:
		renderMode = MeshData::RENDER_MODE::MODE_TRIANGLES_STRIP;
		break;

	case TINYGLTF_MODE_TRIANGLE_FAN:
		renderMode = MeshData::RENDER_MODE::MODE_TRIANGLES_FAN;
		break;


	default:
		assert(false);
		renderMode = MeshData::RENDER_MODE::DEFAULT;
		break;

	}

	return renderMode;
}
