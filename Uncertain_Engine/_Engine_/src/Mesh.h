#pragma once

#include <d3d11.h>
#include <map>
#include <vector>


struct Azul_Vertex;
struct StandardVertex;
struct TriangleByIndex;
class TextureObject;

class Mesh
{
private:
	struct MeshIndexData
	{
		int Offset;
		int NumTri;
		TextureObject* pTexture;

		MeshIndexData()
			: Offset(0), NumTri(0), pTexture(nullptr)
		{
		}
		MeshIndexData(int numtri)
			: Offset(0), NumTri(numtri), pTexture(nullptr)
		{ }
		MeshIndexData(int offset, int numtri)
			: Offset(offset), NumTri(numtri), pTexture(nullptr)
		{ }
	};

	std::vector<MeshIndexData> MeshData;

public:
	Mesh();
	Mesh(const Mesh&) = delete;				   // Copy constructor
	Mesh(Mesh&&) = default;                    // Move constructor
	Mesh& operator=(const Mesh&) & = default;  // Copy assignment operator
	Mesh& operator=(Mesh&&) & = default;       // Move assignment operator
	
	Mesh(const Azul_Vertex* const pAzulData, TriangleByIndex*& pTris, int numTris);
	Mesh(int numTris);
	

	void RenderAll(ID3D11DeviceContext* context) const;
	void RenderMesh(ID3D11DeviceContext* context, int meshNum = 0) const;

	int GetMeshCount() const { return MeshData.size(); }

};
