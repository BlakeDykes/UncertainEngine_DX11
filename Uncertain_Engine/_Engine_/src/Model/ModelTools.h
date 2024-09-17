// ModelTools
// Support tools to compute bounding volumes on models
// Ported/adapted from Keenan's FBX Converter code
// Andre Berthiaume, June 2016

#ifndef _ModelTools
#define _ModelTools

#include "Vec3.h"

struct StandardVertex;
struct TriangleByIndex;

class ModelTools
{

public:
	static void CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&poTris, int& ntri, float scale);
	static void CreateUnitBoxInverted(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& poTris, int& ntri, float scale);
	static void CreateUnitPyramid(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&poTris, int& ntri, float scale);
	static void CreateUnitSphere(int vslice, int hslice, StandardVertex *&pVerts, int& nverts, TriangleByIndex *&poTris, int& ntri, float scale);
	static void CreateFlatPlane(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& poTris, int& ntri, float len, float urep, float vrep);

	static void SetFaceNormal(StandardVertex* pV0, StandardVertex* pV1, StandardVertex* pV2, Vec3& norm);
	static void AccumulateFaceNormal(StandardVertex* pV0, StandardVertex* pV1, StandardVertex* pV2, Vec3& norm);
	static void NormalizeFaceNormals(StandardVertex* pVerts, int nverts);
};




#endif _ModelTools