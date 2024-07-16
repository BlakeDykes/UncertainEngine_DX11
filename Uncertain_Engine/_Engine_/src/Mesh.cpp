#include "Mesh.h"
#include "Model.h"
#include "TextureObject.h"

Mesh::Mesh()
	: MeshData()
{
}

Mesh::Mesh(const Azul_Vertex* const pAzulData, TriangleByIndex*& pTris, int numTris)
	: MeshData()
{
	// create a temporary map to sort the indices
	using KEY = int;
	using MESHINDICES = std::vector<TriangleByIndex*>;
	std::map<KEY, MESHINDICES> meshes;

	// Create the different list of indices for meshes
	for (int i = 0; i < numTris; ++i)
	{
		TriangleByIndex* ind = &pTris[i];

		// triangles in one mesh shouldn't share vertices with another mesh
		assert((pAzulData[ind->v0].meshNum == pAzulData[ind->v1].meshNum) && (pAzulData[ind->v0].meshNum == pAzulData[ind->v2].meshNum));

		KEY key = (KEY)pAzulData[ind->v0].meshNum;
		meshes[key].push_back(ind);
	}

	// Reconsruct a new tri index list ordered by mesh number
	int meshCount = meshes.size();
	this->MeshData.resize(meshCount);
	int offsetval = 0;

	std::map<KEY, MESHINDICES>::iterator it = meshes.begin();

	for (size_t i = 0; i < (size_t)meshCount; ++i, ++it)
	{
		MeshData[i].NumTri = it->second.size();
		MeshData[i].Offset = offsetval;

		// Collect all the tri indices for the i'th mesh
		size_t j = 0;
		for (TriangleByIndex* tri : it->second)
		{
			pTris[offsetval + j] = *tri;
			++j;
		}

		offsetval += MeshData[i].NumTri;
	}
}

Mesh::Mesh(int numTris)
	: MeshData()
{
	this->MeshData.resize(1);
	this->MeshData[0].NumTri = numTris;
}

void Mesh::RenderAll(ID3D11DeviceContext* context) const
{
	for (const MeshIndexData& m : MeshData)
	{
		context->DrawIndexed(m.NumTri * 3, m.Offset * 3, 0);
	}
}

void Mesh::RenderMesh(ID3D11DeviceContext* context, int meshNum) const
{
	assert(meshNum <= 0 && (size_t)meshNum < MeshData.size());
	
	context->DrawIndexed(MeshData[meshNum].NumTri * 3, MeshData[meshNum].Offset * 3, 0);
}
