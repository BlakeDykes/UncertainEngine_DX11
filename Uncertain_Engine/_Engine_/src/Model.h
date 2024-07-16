#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include "Vec4.h"
#include "Mat4.h"
#include "CBuffers.h"
#include "DLink.h"
#include "Mesh.h"

class ShaderObject;
class GameObject;

typedef UINT ModelHandle;

static const UINT		 OFFSET_POS    = 0;
static const DXGI_FORMAT FORMAT_POS    = DXGI_FORMAT_R32G32B32_FLOAT;

static const UINT		 OFFSET_UV     = 12;
static const DXGI_FORMAT FORMAT_UV     = DXGI_FORMAT_R32G32_FLOAT;

static const UINT		 OFFSET_SPECMOD = 12;
static const DXGI_FORMAT FORMAT_SPECMOD = DXGI_FORMAT_R32G32_SINT;

static const UINT		 OFFSET_NORMAL = 20;
static const DXGI_FORMAT FORMAT_NORMAL = DXGI_FORMAT_R32G32B32_FLOAT;

static const UINT	     OFFSET_COLOR  = 32;
static const DXGI_FORMAT FORMAT_COLOR  = DXGI_FORMAT_R32G32B32A32_FLOAT;

static const UINT	     OFFSET_BLEND = 32;
static const DXGI_FORMAT FORMAT_BLEND = DXGI_FORMAT_R32G32B32A32_FLOAT;

static const Vec4 DEFAULT_VERT_COLOR = Vec4(1.0f, 0.0f, 1.0f, 1.0f);

struct StandardVertex : public Align16
{
	float x;
	float y;
	float z;

	float u;
	float v;
	
	float nx;
	float ny;
	float nz;

	float r;
	float g;
	float b;
	float a;

	StandardVertex()
		: x(0.0f), y(0.0f), z(0.0f), 
		u(0.0f), v(0.0f), 
		nx(0.0f), ny(0.0f), nz(0.0f), 
		r(0.0f), g(0.0f), b(0.0f), a(0.0f)
	{ }
	StandardVertex(const StandardVertex&) = default;
	StandardVertex& operator=(const StandardVertex&) = default;
	~StandardVertex() = default;

	Vec3 Pos() const { return Vec3(x, y, z); }
	Vec3 Norm() const { return Vec3(nx, ny, nz); }
	Vec4 Color() const { return Vec4(r, g, b, a); }

	void Set(float _x, float _y, float _z,
		float _u = 0.0f, float _v = 0.0f,
		float _nx = 0.0f, float _ny = 0.0f, float _nz = 0.0f,
		float _r = 1.0f, float _g = 0.0f, float _b = 1.0f, float _a = 1.0f);

	void Set(const Vec3& pos, float _u, float _v, const Vec3& norm, const Vec4& color = DEFAULT_VERT_COLOR);
	void Set(const Vec4& pos, float _u, float _v, const Vec4& norm, const Vec4& color = DEFAULT_VERT_COLOR);
	void Set_PC(float _x, float _y, float _z, const Vec4& c);
	void Set_PC(const Vec3& p, const Vec4& c);
	void Set_PUV(const Vec3& p, float _u, float _v);
	void Set_PUVC(float _x, float _y, float _z, float _u, float _v, const Vec4& c);
	void Set_P(const Vec3& p);
	void Set_PN(float _x, float _y, float _z, float _nx, float _ny, float _nz);
	void Set_UV(float _u, float _v);
	void Set_N(const Vec3& n);
	void Set_Blend(float _r, float _g, float _b, float _a);
	void AddNorm(const Vec3& n);
	void Normalize_N();
};

struct AudioVertex
{
	AudioVertex()
		: x(0.0f), y(0.0f), z(0.0f),
		isSpectrum(-1), spectrumIndex(0),
		nx(0.0f), ny(0.0f), nz(0.0f)
	{
	}

	void Set(float _x, float _y, float _z,
		int isspec, unsigned int specIndex,
		float _nx = 0.0f, float _ny = 0.0f, float _nz = 0.0f
	);

	float x;
	float y;
	float z;

	int isSpectrum;
	unsigned int spectrumIndex;

	float nx;
	float ny;
	float nz;
};

struct TriangleByIndex
{
	unsigned int v0;
	unsigned int v1;
	unsigned int v2;

	void Set(int _v0, int _v1, int _v2)
	{
		v0 = _v0;
		v1 = _v1;
		v2 = _v2;
	}
};

class Model
{

public:
	enum ModelType
	{
		UnitBox,
		UnitBoxInverted,
		UnitPyramid,
		UnitSphere,

		Terrain,
		Skybox,
		FlatPlane,
		AudioSpectrum,

		CustomModel,
		Uninitialized
	};

public:
	ModelType MType;
	Mesh* poMeshes;

	Model();
	Model(const Model&) = delete;				// Copy constructor
	Model(Model&&) = delete;                    // Move constructor
	Model& operator=(const Model&) & = delete;  // Copy assignment operator
	Model& operator=(Model&&) & = delete;       // Move assignment operator
	virtual ~Model();

	Model(ID3D11Device* dev, StandardVertex *pVerts, int nverts, TriangleByIndex *ptlist, int ntri);
	Model(ID3D11Device* dev, ModelType pm, float scale = 1.0f);
	Model(ID3D11Device* dev, int vslice, int hslice, float scale = 1.0f);
	Model(ID3D11Device* dev, const char* const modelName);

	virtual void SetToCurrent(ID3D11DeviceContext* context);
	virtual void Render(ID3D11DeviceContext* context);
	virtual void RenderMesh(ID3D11DeviceContext* context, int meshnum);

	virtual int GetMeshCount() { return poMeshes->GetMeshCount(); }
	bool ValidMeshNum(int i) { return (i >= 0 && i < poMeshes->GetMeshCount()); }

protected:
	TriangleByIndex* poTris;
	StandardVertex* poVerts;
	int numVerts;
	int numTris;

	ID3D11Device* mDevice;
	ID3D11Buffer* mpoVertexBuffer;
	ID3D11Buffer* mpoIndexBuffer;
	char _pad[4] = {};

	void protLoadDataFromFile(const char* const _modelName, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTris, int& ntri);
	virtual void protLoadDataToGPU();
};

struct Azul_Vertex
{
	float x;    // Vert - V
	float y;
	float z;

	float u;    // UV tex coor - U
	float v;

	float meshNum;

	float nx;   // Norm verts - N
	float ny;
	float nz;

	float r;
	float g;
	float b;
};

#endif