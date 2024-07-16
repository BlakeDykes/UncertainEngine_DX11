#ifndef MODEL_DATA_H
#define MODEL_DATA_H

#include "MeshData.h"
#include "ModelData.pb.h"

class ModelData
{
public:
	static const unsigned int MODEL_NAME_SIZE = 64;


	ModelData();
	ModelData(const ModelData& m) = delete;
	ModelData& operator=(const ModelData& m) = delete;
	~ModelData();

	void Serialize(ModelData_proto& out) const;
	void Deserialize(ModelData_proto& in);

	char pModelName[MODEL_NAME_SIZE];
	unsigned int MeshCount;
	MeshData* Meshes;
};

#endif // !MODEL_DATA_H
