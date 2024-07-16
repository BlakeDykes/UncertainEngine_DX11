#ifndef SKINNED_MESH_DATA_H
#define SKINNED_MESH_DATA_H

#include "VBOData.h"
#include "MeshData.h"
#include "BoundingObjectData.h"
#include "SkinnedMeshData.pb.h"


struct SkinningVBOData
{
	VBOData VBOWeigths;
	VBOData VBOJoints;
	VBOData VBOInvBind;
};

class SkinnedMeshData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;

	SkinnedMeshData() = default;
	SkinnedMeshData(const SkinnedMeshData& m) = delete;
	SkinnedMeshData& operator=(const SkinnedMeshData& m) = delete;
	~SkinnedMeshData() = default;

	//void Serialize(SkinnedMeshData_proto& out) const;
	//void Deserialize(SkinnedMeshData_proto& in);

	//void Print() const;

	char pMeshName[FILE_NAME_SIZE];
	unsigned int PrimitiveCount;
	MeshData* Primitives;
	SkinningVBOData* SkinningData;
};

#endif // !SKINNED_MESH_DATA_H
