#include "Model_FlatPlane.h"
#include "ModelTools.h"

Model_FlatPlane::Model_FlatPlane(ID3D11Device* dev, float len, float urep, float vrep)
	: Model(dev, FlatPlane)
{
	ModelTools::CreateFlatPlane(poVerts, numVerts, poTris, numTris, len, urep, vrep);
	this->poMeshes = new Mesh(numTris);
	protLoadDataToGPU();
}
