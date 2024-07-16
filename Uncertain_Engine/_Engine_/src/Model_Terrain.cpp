#include "Model_Terrain.h"
#include "d3dUtil.h"
#include "TextureUtils.h"
#include "ModelTools.h"
#include "Utils.h"

Model_Terrain::Model_Terrain(ID3D11Device* dev, LPCWSTR heightMapFP,
	float modelLength, float maxHeight, float yTrans, int uRep, int vRep)
	: Model(dev, Terrain), URep(uRep), VRep(vRep), ModelLength(modelLength),
	MaxHeight(maxHeight), YTranslation(yTrans)
{
	float largeIncrement = MaxHeight * .2f;
	float smallIncrement = largeIncrement * .5f;

	SandBreakpoint = yTrans + largeIncrement;
	GrassBreakpoint = SandBreakpoint + smallIncrement + smallIncrement * .3f;
	RockBreakpoint = GrassBreakpoint + largeIncrement * 3;

	DirectX::ScratchImage srcTex;
	HRESULT hr = LoadFromTGAFile(heightMapFP, nullptr, srcTex);
	assert(SUCCEEDED(hr));

	const DirectX::Image* heightMap = srcTex.GetImage(0, 0, 0);
	assert(heightMap->height == heightMap->width);

	GenTerrain(heightMap);

	this->poMeshes = new Mesh(numTris);
	protLoadDataToGPU();
}

void Model_Terrain::GenTerrain(const DirectX::Image* heightMap)
{
	size_t texLength = heightMap->height;
	size_t pixel_width = 4;

	this->numVerts = texLength * texLength;
	this->poVerts = new StandardVertex[numVerts];

	this->numTris = (texLength - 1) * (texLength - 1) * 2;
	this->poTris = new TriangleByIndex[numTris];

	// x & z position vars with respect to ModelLength and texLength
	float originIncrement = -2.0f * ModelLength / texLength;
	float halfRect = originIncrement * -0.5f;
	float rectXStart = ModelLength - halfRect;

	// u & v position vars
	float uIncrement = 1.0f / ((float)texLength - 1.0f) * (float)URep;
	float vIncrement = 1.0f / ((float)texLength - 1.0f) * (float)VRep;

	uint8_t* iHeight = heightMap->pixels;

	// Top row of verts (i, height, +z)
	for (size_t i = 0; i < texLength; i += 2)
	{
		float rectXOrigin = rectXStart + (originIncrement * (float)i);

		poVerts[i].Set(    rectXOrigin + halfRect, GetHeight(*iHeight),                 rectXStart + halfRect, (float)i * uIncrement, 0);
		poVerts[i + 1].Set(rectXOrigin - halfRect, GetHeight(*(iHeight + pixel_width)), rectXStart + halfRect, (float)i * uIncrement + uIncrement, 0);

		iHeight += pixel_width * 2;
	}

	Vec3 norm;
	// tris, remaining verts, and face normal accumulation
	for (
		size_t i = texLength, triIndex = 0,  topLeftVert = 0;
		i < (size_t)this->numVerts;
		i += 2,               triIndex += 2, topLeftVert += 2
		)
	{
		size_t xNumber = i % texLength;
		size_t zNumber = (int)(i / texLength);
		size_t texIndex = TexelIndex(pixel_width, texLength, xNumber, zNumber);

		float rectXOrigin = rectXStart + (originIncrement * xNumber);
		float rectZOrigin = rectXStart + (originIncrement * zNumber);

		poVerts[i].Set(    rectXOrigin + halfRect, GetHeight(heightMap->pixels[texIndex]),               rectZOrigin - halfRect, (float)xNumber * uIncrement,              vIncrement * (float)zNumber);
		poVerts[i + 1].Set(rectXOrigin - halfRect, GetHeight(heightMap->pixels[texIndex + pixel_width]), rectZOrigin - halfRect, (float)xNumber * uIncrement + uIncrement, vIncrement * (float)zNumber);

		// if not first column, set indices for previous column
		if (rectXOrigin != rectXStart)
		{
			this->poTris[triIndex++].Set(topLeftVert - 1, i - 1,       i);
			this->poTris[triIndex++].Set(i,               topLeftVert, topLeftVert - 1);

			ModelTools::AccumulateFaceNormal(&poVerts[topLeftVert - 1], &poVerts[i - 1],	   &poVerts[i],				  norm);
			ModelTools::AccumulateFaceNormal(&poVerts[i],				&poVerts[topLeftVert], &poVerts[topLeftVert - 1], norm);
		}

		this->poTris[triIndex].Set(    topLeftVert, i,               i + 1);
		this->poTris[triIndex + 1].Set(i + 1,       topLeftVert + 1, topLeftVert);

		ModelTools::AccumulateFaceNormal(&poVerts[topLeftVert], &poVerts[i],			   &poVerts[i + 1],		  norm);
		ModelTools::AccumulateFaceNormal(&poVerts[i + 1],		&poVerts[topLeftVert + 1], &poVerts[topLeftVert], norm);
	}

	ModelTools::NormalizeFaceNormals(poVerts, numVerts);

	GenBlendValues();
}

float LerpClamped(float v0, float v1, float t, float rangeMax = 1.0f, float rangeMin = 0.0f) { if (t > v1) return rangeMax; else { return (t - v0) / (v1 - v0) * (rangeMax - rangeMin) + rangeMin; } }

void Model_Terrain::GenBlendValues()
{
	for (size_t i = 0; i < (size_t)this->numVerts; ++i)
	{
		StandardVertex& vert = poVerts[i];
		float y = vert.Pos().y();
		Vec3 norm = vert.Norm();
		float xProjLength = fabs(norm.dot(Vec3(1.0f, 0.0f, 0.0f)));

		if (y <= TERRAIN_MIN_Y)
		{
			vert.Set_Blend(1.0f, 1.0f, 0.0f, 0.0f);
		}
		else if (y < SandBreakpoint)
		{
			vert.Set_Blend(1.0f, 0.0f, 0.0f, 0.0f);
		}
		else if (y < GrassBreakpoint)
		{
			if (xProjLength > 0.5f)
			{
				float b = LerpClamped(0.4f, 1.0f, xProjLength);

				vert.Set_Blend(
					0.0f,
					b,
					1.0f - b,
					0.0f
				);
			}
			else
			{
				
				float g = LerpClamped(SandBreakpoint, GrassBreakpoint, y);

				vert.Set_Blend(
					1.0f - g,
					g,
					0.0f,
					0.0f
				);
			}
		}
		else if (y < RockBreakpoint)
		{
			if (xProjLength > 0.5f)
			{
				float b = LerpClamped(0.5f, 1.0f, xProjLength);

				vert.Set_Blend(
					0.0f,
					0.0f,
					b,
					1.0f - b
				);
			}
			else
			{
				float b = LerpClamped(GrassBreakpoint, RockBreakpoint, y);

				vert.Set_Blend(
					0.0f,
					1.0f - b,
					b,
					0.0f
				);
			}
		}
		else
		{
			vert.Set_Blend(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}
}

float Model_Terrain::GetHeight(uint8_t texVal)
{
	float ret = YTranslation + ((float)texVal * MaxHeight) / 255.0f;

	return ret <= TERRAIN_MIN_Y ? TERRAIN_MIN_Y : ret;
}


