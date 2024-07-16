#include "ModelTools.h"
//#include "Enum.h"
//#include <math.h>
#include <assert.h>
#include "Model.h"
#include "d3dUtil.h"


/// Creates the unit box centered at the origin
void ModelTools::CreateUnitBox(StandardVertex *&pVerts, int& nverts, TriangleByIndex *&poTris, int& ntri, float scale)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	poTris = new TriangleByIndex[ntri];

	float posHalfToScale = 0.5f * scale;
	float negHalfToScale = posHalfToScale * -1.0f;

				  //x			  y				  z				  u      v       nx    ny    nz
	pVerts[0].Set(posHalfToScale, negHalfToScale, negHalfToScale, 0.75f, 0.625f, 0.0f, 0.0f, -1.0f);	// <-- Back
	pVerts[1].Set(posHalfToScale, posHalfToScale, negHalfToScale, 0.5f,  0.625f, 0.0f, 0.0f, -1.0f);
	pVerts[2].Set(negHalfToScale, posHalfToScale, negHalfToScale, 0.5f,  0.375f, 0.0f, 0.0f, -1.0f);
	pVerts[3].Set(negHalfToScale, negHalfToScale, negHalfToScale, 0.75f, 0.375f, 0.0f, 0.0f, -1.0f);

	pVerts[4].Set(posHalfToScale, negHalfToScale, posHalfToScale, 0.0f,  0.625f, 0.0f, 0.0f, 1.0f);		// <-- Front
	pVerts[5].Set(posHalfToScale, posHalfToScale, posHalfToScale, 0.25,  0.625f, 0.0f, 0.0f, 1.0f);
	pVerts[6].Set(negHalfToScale, posHalfToScale, posHalfToScale, 0.25f, 0.375f, 0.0f, 0.0f, 1.0f);
	pVerts[7].Set(negHalfToScale, negHalfToScale, posHalfToScale, 0.0f,  0.375f, 0.0f, 0.0f, 1.0f);

	pVerts[8].Set(posHalfToScale, negHalfToScale, posHalfToScale,  0.25f, 0.875f, 1.0f, 0.0f, 0.0f);	// <-- Left
	pVerts[9].Set(posHalfToScale, negHalfToScale, negHalfToScale,  0.5f,  0.875f, 1.0f, 0.0f, 0.0f);
	pVerts[10].Set(posHalfToScale, posHalfToScale, negHalfToScale, 0.5f,  0.625f, 1.0f, 0.0f, 0.0f);
	pVerts[11].Set(posHalfToScale, posHalfToScale, posHalfToScale, 0.25f, 0.625f, 1.0f, 0.0f, 0.0f);

	pVerts[12].Set(negHalfToScale, negHalfToScale, negHalfToScale, 0.5f,  0.125f, -1.0f, 0.0f, 0.0f);	// <-- Right
	pVerts[13].Set(negHalfToScale, negHalfToScale, posHalfToScale, 0.25f, 0.125f, -1.0f, 0.0f, 0.0f);
	pVerts[14].Set(negHalfToScale, posHalfToScale, posHalfToScale, 0.25f, 0.375f, -1.0f, 0.0f, 0.0f);
	pVerts[15].Set(negHalfToScale, posHalfToScale, negHalfToScale, 0.5f,  0.375f, -1.0f, 0.0f, 0.0f);

	pVerts[16].Set(negHalfToScale, posHalfToScale, posHalfToScale, 0.25f, 0.375f, 0.0f, 1.0f, 0.0f);	// <-- Top
	pVerts[17].Set(posHalfToScale, posHalfToScale, posHalfToScale, 0.25f, 0.625f, 0.0f, 1.0f, 0.0f);
	pVerts[18].Set(posHalfToScale, posHalfToScale, negHalfToScale, 0.5f,  0.625f, 0.0f, 1.0f, 0.0f);
	pVerts[19].Set(negHalfToScale, posHalfToScale, negHalfToScale, 0.5f,  0.375f, 0.0f, 1.0f, 0.0f);

	pVerts[20].Set(posHalfToScale, negHalfToScale, posHalfToScale, 0.75f, 0.375f, 0.0f, -1.0f, 0.0f);	// <-- Bottom
	pVerts[21].Set(negHalfToScale, negHalfToScale, posHalfToScale, 0.75f, 0.625f, 0.0f, -1.0f, 0.0f);
	pVerts[22].Set(negHalfToScale, negHalfToScale, negHalfToScale, 1.0f,  0.625f, 0.0f, -1.0f, 0.0f);
	pVerts[23].Set(posHalfToScale, negHalfToScale, negHalfToScale, 1.0f,  0.375f, 0.0f, -1.0f, 0.0f);

	// back face
	poTris[0].Set(2, 1, 0);
	poTris[1].Set(0, 3, 2);

	// front face
	poTris[2].Set(4, 5, 6);
	poTris[3].Set(6, 7, 4);

	// left face
	poTris[4].Set(8, 9, 10);
	poTris[5].Set(10, 11, 8);

	// right face
	poTris[6].Set(12, 13, 14);
	poTris[7].Set(14, 15, 12);

	// top face
	poTris[8].Set(16, 17, 18);
	poTris[9].Set(18, 19, 16);

	// bottom face
	poTris[10].Set(20, 21, 22);
	poTris[11].Set(22, 23, 20);
}

void ModelTools::CreateUnitBoxInverted(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& poTris, int& ntri, float scale)
{
	nverts = 24;
	pVerts = new StandardVertex[nverts];
	ntri = 12;
	poTris = new TriangleByIndex[ntri];

	float posHalfToScale = 0.5f * scale;
	float negHalfToScale = posHalfToScale * -1.0f;

	// Front
	pVerts[0].Set_PN(posHalfToScale, negHalfToScale, negHalfToScale, 0.0f, 0.0f, 1.0f);
	pVerts[1].Set_PN(posHalfToScale, posHalfToScale, negHalfToScale, 0.0f, 0.0f, 1.0f);
	pVerts[2].Set_PN(negHalfToScale, posHalfToScale, negHalfToScale, 0.0f, 0.0f, 1.0f);
	pVerts[3].Set_PN(negHalfToScale, negHalfToScale, negHalfToScale, 0.0f, 0.0f, 1.0f);

	// Back
	pVerts[4].Set_PN(posHalfToScale, negHalfToScale, posHalfToScale, 0.0f, 0.0f, -1.0f);
	pVerts[5].Set_PN(posHalfToScale, posHalfToScale, posHalfToScale, 0.0f, 0.0f, -1.0f);
	pVerts[6].Set_PN(negHalfToScale, posHalfToScale, posHalfToScale, 0.0f, 0.0f, -1.0f);
	pVerts[7].Set_PN(negHalfToScale, negHalfToScale, posHalfToScale, 0.0f, 0.0f, -1.0f);

	// Left
	pVerts[8].Set_PN(posHalfToScale, negHalfToScale, posHalfToScale, -1.0f, 0.0f, 0.0f);
	pVerts[9].Set_PN(posHalfToScale, negHalfToScale, negHalfToScale, -1.0f, 0.0f, 0.0f);
	pVerts[10].Set_PN(posHalfToScale, posHalfToScale, negHalfToScale, -1.0f, 0.0f, 0.0f);
	pVerts[11].Set_PN(posHalfToScale, posHalfToScale, posHalfToScale, -1.0f, 0.0f, 0.0f);

	// Right
	pVerts[12].Set_PN(negHalfToScale, negHalfToScale, negHalfToScale, 1.0f, 0.0f, 0.0f);
	pVerts[13].Set_PN(negHalfToScale, negHalfToScale, posHalfToScale, 1.0f, 0.0f, 0.0f);
	pVerts[14].Set_PN(negHalfToScale, posHalfToScale, posHalfToScale, 1.0f, 0.0f, 0.0f);
	pVerts[15].Set_PN(negHalfToScale, posHalfToScale, negHalfToScale, 1.0f, 0.0f, 0.0f);

	// Top
	pVerts[16].Set_PN(negHalfToScale, posHalfToScale, posHalfToScale, 0.0f, -1.0f, 0.0f);
	pVerts[17].Set_PN(posHalfToScale, posHalfToScale, posHalfToScale, 0.0f, -1.0f, 0.0f);
	pVerts[18].Set_PN(posHalfToScale, posHalfToScale, negHalfToScale, 0.0f, -1.0f, 0.0f);
	pVerts[19].Set_PN(negHalfToScale, posHalfToScale, negHalfToScale, 0.0f, -1.0f, 0.0f);

	// Bottom
	pVerts[20].Set_PN(posHalfToScale, negHalfToScale, posHalfToScale, 0.0f, 1.0f, 0.0f);
	pVerts[21].Set_PN(negHalfToScale, negHalfToScale, posHalfToScale, 0.0f, 1.0f, 0.0f);
	pVerts[22].Set_PN(negHalfToScale, negHalfToScale, negHalfToScale, 0.0f, 1.0f, 0.0f);
	pVerts[23].Set_PN(posHalfToScale, negHalfToScale, negHalfToScale, 0.0f, 1.0f, 0.0f);

	// front face
	poTris[0].Set(0, 1, 2);
	poTris[1].Set(2, 3, 0);

	// back face
	poTris[2].Set(6, 5, 4);
	poTris[3].Set(4, 7, 6);

	// left face
	poTris[4].Set(10, 9, 8);
	poTris[5].Set(8, 11, 10);

	// right face
	poTris[6].Set(14, 13, 12);
	poTris[7].Set(12, 15, 14);

	// top face
	poTris[8].Set(18, 17, 16);
	poTris[9].Set(16, 19, 18);

	// bottom face
	poTris[10].Set(22, 21, 20);
	poTris[11].Set(20, 23, 22);
}


void ModelTools::CreateUnitPyramid(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& poTris, int& ntri, float scale)
{
	nverts = 18;
	pVerts = new StandardVertex[nverts];
	ntri = 6;
	poTris = new TriangleByIndex[ntri];


	float posHalfToScale = 0.5f * scale;
	float negHalfToScale = posHalfToScale * -1.0f;


	pVerts[0].Set(posHalfToScale, negHalfToScale, negHalfToScale, scale, scale);
	pVerts[1].Set(0.0f,           posHalfToScale, 0.0f,           0.5f,  0.0f);
	pVerts[2].Set(posHalfToScale, negHalfToScale, posHalfToScale, 0.0f,  scale);

	pVerts[3].Set(negHalfToScale, negHalfToScale, posHalfToScale, scale, scale);
	pVerts[4].Set(+0.0f,          posHalfToScale, +0.0f,          0.5f,  0.0f);
	pVerts[5].Set(negHalfToScale, negHalfToScale, negHalfToScale, 0.0f,  scale);

	pVerts[6].Set(posHalfToScale, negHalfToScale, posHalfToScale, scale, scale);
	pVerts[7].Set(+0.0f,          posHalfToScale, 0.0f,           0.5f,  0.0f);
	pVerts[8].Set(negHalfToScale, negHalfToScale, posHalfToScale, 0.0f,  scale);

	pVerts[9].Set(negHalfToScale,  negHalfToScale, negHalfToScale, scale, scale);
	pVerts[10].Set(+0.0f,          posHalfToScale, 0.0f,           0.5f,  0.0f);
	pVerts[11].Set(posHalfToScale, negHalfToScale, negHalfToScale, 0.0f,  scale);

	pVerts[12].Set(negHalfToScale, negHalfToScale, negHalfToScale, scale, 0.0f);
	pVerts[13].Set(posHalfToScale, negHalfToScale, negHalfToScale, 0.0f, 0.0f);
	pVerts[14].Set(negHalfToScale, negHalfToScale, posHalfToScale, scale, scale);

	pVerts[15].Set(posHalfToScale, negHalfToScale, negHalfToScale, 0.0f, 0.0f);
	pVerts[16].Set(posHalfToScale, negHalfToScale, posHalfToScale, 0.0f, scale);
	pVerts[17].Set(negHalfToScale, negHalfToScale, posHalfToScale, scale, scale);


	poTris[0].Set(0, 1, 2);
	poTris[1].Set(3, 4, 5);
	poTris[2].Set(6, 7, 8);
	poTris[3].Set(9, 10, 11);
	poTris[4].Set(12, 13, 14);
	poTris[5].Set(15, 16, 17);

	Vec3 norm;
	for (int i = 0; i < ntri; ++i)
	{
		SetFaceNormal(&pVerts[poTris[i].v0], &pVerts[poTris[i].v1], &pVerts[poTris[i].v2], norm);
	}

}

void ModelTools::CreateUnitSphere(int vslice, int hslice, StandardVertex*& pVerts, int& nverts, TriangleByIndex*& poTris, int& ntri, float scale)
{
	nverts = (vslice + 1) * hslice + 2;
	pVerts = new StandardVertex[nverts];

	ntri = (vslice * (hslice - 1) * 2) + (vslice * 2);
	poTris = new TriangleByIndex[ntri];

	float deltaPhi = MATH_PI * scale / (hslice + 1);
	float deltaTheta = MATH_2PI * scale / vslice;

	// u & v position vars
	float deltaU = 1.0f / (float)vslice;
	float deltaV = 1.0f / (float)hslice;

	Mat4 rotX(Mat4::Rot1::X, sinf(deltaPhi) * cosf(deltaTheta));
	Mat4 rotY(Mat4::Rot1::Y, deltaTheta);
	Mat4 rotZ(Mat4::Rot1::Z, sinf(deltaPhi) * sinf(deltaTheta));

	Vec4 pos(0.0f, scale, 0.0f);
	pVerts[0].Set(pos, 0.5f, 0.0f, pos);

	for (int i = 0, index = 1; i < hslice; ++i)
	{
		pos *= rotZ * rotX;

		pVerts[index++].Set(pos, 0.0f, (float)(i * deltaV), pos.getNorm());

		for (int j = 0; j < vslice; ++j)
		{
			pos *= rotY;
			pVerts[index++].Set(pos, 
								(float)((j + 1) * deltaU), (float)(i * deltaV),
								pos.getNorm());
		}
	}

	pVerts[nverts - 1].Set(0.0f, -scale, 0.0f, 
						   0.5f, 1.0f, 
						   0.0f, -1.0f, 0.0f);

	// Set Indices
	// v0--v01
	// |  /|  
	// | / |
	// v1---v11
	//
	unsigned int startV0, startV1, v0, v1, vSliceInc;
	for (int i = 0, index = 0; i < hslice; ++i)
	{
		startV1 = i * (vslice + 1) + 1;
		startV0 = startV1 - (vslice + 1);

		// Middle hslices
		if (i != 0 && i != hslice - 1)
		{
			v0 = startV0;
			v1 = startV1;
			for (int j = 0; j < vslice; ++j, v0++, v1++)
			{
				vSliceInc = ((j + 1) % (vslice + 1));
				poTris[index++].Set(v0,			         v1,                  startV0 + vSliceInc);
				poTris[index++].Set(startV1 + vSliceInc, startV0 + vSliceInc, v1                 );
			}
		}
		// Topmost hslice
		else if (i == 0)
		{
			v1 = startV1;

			for (int j = 0; j < vslice; ++j, v1++)
			{
				vSliceInc = ((j + 1) % (vslice + 1));

				poTris[index++].Set(0, v1, startV1 + vSliceInc);
			}
		}
		// Bottom hslice
		else if (i == hslice - 1)
		{
			v0 = startV0;
			v1 = startV1;

			for (int j = 0; j < vslice; ++j, v0++, v1++)
			{
				vSliceInc = ((j + 1) % (vslice + 1));

				poTris[index++].Set(v0, nverts - 1, startV0 + vSliceInc);
			}
		}
	}
}

void ModelTools::CreateFlatPlane(StandardVertex*& pVerts, int& nverts, TriangleByIndex*& pTris, int& ntri, float len, float urep, float vrep)
{
	nverts = 4;
	pVerts = new StandardVertex[nverts];

	pVerts[0].Set(-0.5f * len, 0.0f, -0.5f * len, urep, vrep, 0.0f, 1.0f, 0.0f);
	pVerts[1].Set(-0.5f * len, 0.0f, 0.5f * len, urep, 0, 0.0f, 1.0f, 0.0f);
	pVerts[2].Set(0.5f * len, 0.0f, 0.5f * len, 0, 0, 0.0f, 1.0f, 0.0f);
	pVerts[3].Set(0.5f * len, 0.0f, -0.5f * len, 0, vrep, 0.0f, 1.0f, 0.0f);

	ntri= 2;
	pTris = new TriangleByIndex[ntri];

	pTris[0].Set(0, 1, 2);
	pTris[1].Set(0, 2, 3);
}

void ModelTools::SetFaceNormal(StandardVertex* pV0, StandardVertex* pV1, StandardVertex* pV2, Vec3& norm)
{
	assert(pV0);
	assert(pV1);
	assert(pV2);
	norm.set((pV1->Pos() - pV0->Pos()).cross(pV2->Pos() - pV0->Pos()));
	norm.norm();
	//norm.set(Vec3(norm.x(), norm.y(), norm.z()).norm());

	pV0->Set_N(norm);
	pV1->Set_N(norm);
	pV2->Set_N(norm);
}

void ModelTools::AccumulateFaceNormal(StandardVertex* pV0, StandardVertex* pV1, StandardVertex* pV2, Vec3& norm)
{
	assert(pV0);
	assert(pV1);
	assert(pV2);
	norm.set((pV1->Pos() - pV0->Pos()).cross(pV2->Pos() - pV0->Pos()));
	norm.norm();
	//norm.set(norm.X(), norm.Y(), norm.Z(), 0.0f);

	pV0->AddNorm(norm);
	pV1->AddNorm(norm);
	pV2->AddNorm(norm);
}

void ModelTools::NormalizeFaceNormals(StandardVertex* pVerts, int nverts)
{
	assert(pVerts);
	assert(nverts >= 3);

	for (int i = 0; i < nverts; ++i)
	{
		pVerts[i].Normalize_N();
	}
}





