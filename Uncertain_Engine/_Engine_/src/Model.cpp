#include <assert.h>
#include "File.h"
#include "Model.h"
#include "ModelTools.h"
#include "d3dUtil.h"
#include "ShaderObject.h"
#include "GameObject.h"
#include "AzulFileHdr.h"

Model::Model()
	: MType(Uninitialized), poMeshes(nullptr), poTris(nullptr), poVerts(nullptr),
	numVerts(0), numTris(0), mDevice(nullptr), mpoVertexBuffer(nullptr), mpoIndexBuffer(nullptr)
{
}

Model::Model(ID3D11Device* dev, StandardVertex* pVerts, int nverts, TriangleByIndex* ptlist, int ntri)
	: MType(CustomModel), poTris(new TriangleByIndex[ntri]), poVerts(new StandardVertex[numVerts]), 
	numVerts(nverts), numTris(ntri), mDevice(dev)
{
	assert(nverts > 0);
	assert(ntri > 0);

	// Copy Data
	for (int i = 0; i < numVerts; i++)
	{
		poVerts[i] = pVerts[i];
	}
	for (int i = 0; i < numTris; i++)
	{
		poTris[i] = ptlist[i];
	}

	poMeshes = new Mesh(numTris);

	protLoadDataToGPU();
}

Model::Model(ID3D11Device* dev, Model::ModelType pm, float scale)
	: MType(pm), poMeshes(nullptr), poTris(nullptr), poVerts(nullptr),
	numVerts(0), numTris(0), mDevice(dev), mpoVertexBuffer(nullptr), mpoIndexBuffer(nullptr)
{
	switch (pm)
	{
	case UnitBox:
		ModelTools::CreateUnitBox(poVerts, numVerts, poTris, numTris, scale);
		break;
	case UnitBoxInverted:
		ModelTools::CreateUnitBoxInverted(poVerts, numVerts, poTris, numTris, scale);
		break;
	case UnitPyramid:
		ModelTools::CreateUnitPyramid(poVerts, numVerts, poTris, numTris, scale);
		break;
	case UnitSphere:
		ModelTools::CreateUnitSphere(5, 5, poVerts, numVerts, poTris, numTris, scale);
		break;

	case Skybox:
		ModelTools::CreateUnitBoxInverted(poVerts, numVerts, poTris, numTris, scale);
		return;

	case Terrain:
	case FlatPlane:
	case CustomModel:
	case Uninitialized:
	default:
		return;
	}

	poMeshes = new Mesh(numTris);

	protLoadDataToGPU();
}

Model::Model(ID3D11Device* dev, int vslice, int hslice, float scale)
	: MType(ModelType::UnitSphere), mDevice(dev)
{
	ModelTools::CreateUnitSphere(vslice, hslice, poVerts, numVerts, poTris, numTris, scale);
	
	poMeshes = new Mesh(numTris);

	protLoadDataToGPU();
}

Model::Model(ID3D11Device* dev, const char* const modelName)
	: MType(CustomModel), mDevice(dev)
{
	protLoadDataFromFile(modelName, poVerts, numVerts, poTris, numTris);
	protLoadDataToGPU();
}

Model::~Model()
{
	delete[] poTris;
	delete[] poVerts;

	ReleaseAndDeleteCOMobject(mpoVertexBuffer);
	ReleaseAndDeleteCOMobject(mpoIndexBuffer);

	delete poMeshes;
}

void Model::SetToCurrent(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(StandardVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mpoVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mpoIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Render(ID3D11DeviceContext* context)
{
	this->poMeshes->RenderAll(context);
}

void Model::RenderMesh(ID3D11DeviceContext* context, int meshNum)
{
	this->poMeshes->RenderMesh(context, meshNum);
}

void Model::protLoadDataFromFile(const char* const _modelName, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTris, int& ntri)
{
	// Read from file
	File::Handle fh;
	File::Error  ferror;

	ferror = File::Open(fh, _modelName, File::Mode::READ);
	assert(ferror == File::Error::SUCCESS);

	// Get the file format for the data
	AzulFileHdr  azulFileHdr;

	// Read the header
	ferror = File::Read(fh, &azulFileHdr, sizeof(AzulFileHdr));
	assert(ferror == File::Error::SUCCESS);

	// create the vertex buffer
	nverts = azulFileHdr.numVerts;
	Azul_Vertex* pAzulVerts = new Azul_Vertex[nverts];

	// load the verts
	// seek to the location
	ferror = File::Seek(fh, File::Position::BEGIN, azulFileHdr.vertBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	// read it
	ferror = File::Read(fh, pAzulVerts, nverts * sizeof(Azul_Vertex));
	assert(ferror == File::Error::SUCCESS);

	// create the triLists buffer
	ntri = azulFileHdr.numTriangles;
	pTris = new TriangleByIndex[ntri];

	// load the triList
	// seek to the location
	ferror = File::Seek(fh, File::Position::BEGIN, azulFileHdr.triangleListBufferOffset);
	assert(ferror == File::Error::SUCCESS);

	// read it
	ferror = File::Read(fh, pTris, ntri * sizeof(TriangleByIndex));
	assert(ferror == File::Error::SUCCESS);

	// close
	ferror = File::Close(fh);
	assert(ferror == File::Error::SUCCESS);

	pVerts = new StandardVertex[nverts];
	
	Azul_Vertex* pCur;
	for (int i = 0; i < nverts; i++)
	{
		pCur = &(pAzulVerts[i]);
		pVerts[i].Set(pCur->x, pCur->y, pCur->z, pCur->u, pCur->v, pCur->nx, pCur->ny, pCur->nz, pCur->r, pCur->g, pCur->b);
	}

	poMeshes = new Mesh(pAzulVerts, poTris, numTris);

	delete[] pAzulVerts;
}

void  Model::protLoadDataToGPU()
{
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(StandardVertex) * numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = poVerts;
	HRESULT hr = mDevice->CreateBuffer(&bd, &InitData, &mpoVertexBuffer);
	assert(SUCCEEDED(hr));

	// Index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TriangleByIndex) * numTris;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = poTris;
	hr = mDevice->CreateBuffer(&bd, &InitData, &mpoIndexBuffer);
	assert(SUCCEEDED(hr));
}


void StandardVertex::Set(float _x, float _y, float _z,
	float _u, float _v,
	float _nx, float _ny, float _nz,
	float _r, float _g, float _b, float _a)
{
	x = _x;
	y = _y;
	z = _z;
	u = _u;
	v = _v;
	nx = _nx;
	ny = _ny;
	nz = _nz;
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

void StandardVertex::Set(const Vec3& pos, float _u, float _v, const Vec3& norm, const Vec4& color)
{
	x = pos.x();
	y = pos.y();
	z = pos.z();
	u = _u;
	v = _v;
	nx = norm.x();
	ny = norm.y();
	nz = norm.z();
	r = color.x();
	g = color.y();
	b = color.z();
}

void StandardVertex::Set(const Vec4& pos, float _u, float _v, const Vec4& norm, const Vec4& color)
{
	x = pos.x();
	y = pos.y();
	z = pos.z();
	u = _u;
	v = _v;
	nx = norm.x();
	ny = norm.y();
	nz = norm.z();
	r = color.x();
	g = color.y();
	b = color.z();
}

void StandardVertex::Set_PC(float _x, float _y, float _z, const Vec4& c)
{
	x = _x;
	y = _y;
	z = _z;
	r = c.x();
	g = c.y();
	b = c.z();
}

void StandardVertex::Set_PC(const Vec3& p, const Vec4& c)
{
	x = p.x();
	y = p.y();
	z = p.z();
	r = c.x();
	g = c.y();
	b = c.z();
	a = c.w();
}

void StandardVertex::Set_PUV(const Vec3& p, float _u, float _v)
{
	x = p.x();
	y = p.y();
	z = p.z();
	u = _u;
	v = _v;
}

void StandardVertex::Set_PUVC(float _x, float _y, float _z, float _u, float _v, const Vec4& c)
{
	x = _x;
	y = _y;
	z = _z;
	u = _u;
	v = _v;
	r = c.x();
	g = c.y();
	b = c.z();
	a = c.w();
}

void StandardVertex::Set_P(const Vec3& p)
{
	x = p.x();
	y = p.y();
	z = p.z();
}

void StandardVertex::Set_PN(float _x, float _y, float _z, float _nx, float _ny, float _nz)
{
	x = _x;
	y = _y;
	z = _z;
	nx = _nx;
	ny = _ny;
	nz = _nz;
}

void StandardVertex::Set_UV(float _u, float _v)
{
	u = _u;
	v = _v;
}

void StandardVertex::Set_N(const Vec3& n)
{
	nx = n.x();
	ny = n.y();
	nz = n.z();
}

void StandardVertex::Set_Blend(float _r, float _g, float _b, float _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

void StandardVertex::AddNorm(const Vec3& n)
{
	nx += n.x();
	ny += n.y();
	nz += n.z();
}

void StandardVertex::Normalize_N()
{
	float len = nx * nx + ny * ny + nz * nz;

	if (len != 0.0f)
	{
		len = sqrtf(len);
		nx /= len;
		ny /= len;
		nz /= len;
	}
	else
	{
		assert(false);
	}
}

void AudioVertex::Set(float _x, float _y, float _z, int isspec, unsigned int specIndex, float _nx, float _ny, float _nz)
{
	this->x = _x;
	this->y = _y;
	this->z = _z;
	this->nx = _nx;
	this->ny = _ny;
	this->nz = _nz;
	this->isSpectrum = isspec;
	this->spectrumIndex = specIndex;
}
