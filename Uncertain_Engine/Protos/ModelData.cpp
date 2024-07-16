#include "ModelData.h"

ModelData::ModelData()
	:pModelName{ 0 }, MeshCount(0), Meshes(nullptr)
{
}

ModelData::~ModelData()
{
	if (this->Meshes)
	{
		delete[] this->Meshes;
		this->Meshes = nullptr;
	}
}


void ModelData::Serialize(ModelData_proto& out) const
{
	std::string modelName((const char*)this->pModelName, MODEL_NAME_SIZE);
	out.set_pmodelname(modelName);
	out.set_meshcount(this->MeshCount);

	MeshData* pMesh = this->Meshes;
	MeshData_proto* meshProto;

	for (unsigned int i = 0; i < this->MeshCount; i++)
	{
		meshProto = out.add_meshes();
		pMesh->Serialize(*meshProto);
		pMesh++;
	}
}

void ModelData::Deserialize(ModelData_proto& in)
{
	this->MeshCount = in.meshcount();

	if (this->MeshCount > 0)
	{
		if (this->Meshes)
		{
			delete[] this->Meshes;
		}

		this->Meshes = new MeshData[this->MeshCount]();
		MeshData_proto* meshProto = nullptr;
		MeshData* pMesh = this->Meshes;

		for (unsigned int i = 0; i < this->MeshCount; i++)
		{
			meshProto = in.mutable_meshes((int)i);
			pMesh->Deserialize(*meshProto);
			pMesh++;
		}
	}

	memcpy_s(this->pModelName, MODEL_NAME_SIZE, in.pmodelname().data(), MODEL_NAME_SIZE);
}
