#ifndef CONVERTER_MESH_DATA_H
#define CONVERTER_MESH_DATA_H

#include "tiny_gltf.h"
#include "MeshData.h"

class Converter_MeshData
{
public:
	static MeshData::RENDER_MODE GetMode(int GLTF_TINY_VAL);
};

#endif // !CONVERTER_MESH_DATA_H
