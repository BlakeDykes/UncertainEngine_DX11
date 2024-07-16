#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "VBOData.h"
#include "BoundingObjectData.h"
#include "MeshData.pb.h"

class MeshData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;

	enum class RENDER_MODE
	{
		MODE_POINTS = 0,
		MODE_LINE = 1,
		MODE_LINE_LOOP = 2,
		MODE_LINE_STRIP = 3,
		MODE_TRIANGLES = 4,
		MODE_TRIANGLES_STRIP = 5,
		MODE_TRIANGLES_FAN = 6,
		DEFAULT = MODE_TRIANGLES
	};

	struct SkinningData
	{
		VBOData VBOJoints;
		VBOData VBOWeights;
		VBOData VBOInvBind;
	};

	MeshData();
	MeshData(const MeshData& m) = delete;
	MeshData& operator=(const MeshData& m) = delete;
	~MeshData();

	void Serialize(MeshData_proto& out) const;
	void Deserialize(MeshData_proto& in);

	void Print() const;

	char pMeshName[FILE_NAME_SIZE];
	RENDER_MODE RenderMode;
	unsigned int TriCount;
	unsigned int VertCount;
	VBOData VBOVert;
	VBOData VBONorm;
	VBOData VBOUV;
	VBOData VBOIndex;

	SkinningData* poSkinData;

	BoundingObjectData* poBounding;
	unsigned int MaterialIndex;
	unsigned int NodeIndex;

};

#endif // !MESH_DATA_H
