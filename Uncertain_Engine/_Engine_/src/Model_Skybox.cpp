#include "Model_Skybox.h"
#include "ModelTools.h"

Model_Skybox::Model_Skybox(ID3D11Device* dev, SkyboxTextures texName, float scale)
	: Model(dev, Skybox, scale)
{
	switch (texName)
	{
	case SkyboxTextures::Daylight:
		SetDaylightUVs();
		break;
	case SkyboxTextures::Night:
		SetNightUVs();
		break;
	default:
		assert(false);
		break;
	}

	poMeshes = new Mesh(numTris);

	protLoadDataToGPU();
}

void Model_Skybox::SetDaylightUVs()
{
	float third = 1.0f / 3.0f;
	float twoThird = third * 2.0f;

	this->poVerts[0].Set_UV(.5f, twoThird);
	this->poVerts[1].Set_UV(.5f, third);
	this->poVerts[2].Set_UV(.25, third);
	this->poVerts[3].Set_UV(.25f, twoThird);

	this->poVerts[4].Set_UV(.75f, twoThird);
	this->poVerts[5].Set_UV(.75f, third);
	this->poVerts[6].Set_UV(1.0f, third);
	this->poVerts[7].Set_UV(1.0f, twoThird);

	this->poVerts[8].Set_UV(0.75f, twoThird);
	this->poVerts[9].Set_UV(0.5f, twoThird);
	this->poVerts[10].Set_UV(0.5f, third);
	this->poVerts[11].Set_UV(0.755f, third);

	this->poVerts[12].Set_UV(0.25f, twoThird);
	this->poVerts[13].Set_UV(0.0f, twoThird);
	this->poVerts[14].Set_UV(0.0f, third);
	this->poVerts[15].Set_UV(0.25f, third);

	this->poVerts[16].Set_UV(0.25f, 0.0f);
	this->poVerts[17].Set_UV(0.5f, 0.0f);
	this->poVerts[18].Set_UV(0.5f, third);
	this->poVerts[19].Set_UV(0.25f, third);

	this->poVerts[20].Set_UV(0.5f, 1.0f);
	this->poVerts[21].Set_UV(0.25f, 1.0f);
	this->poVerts[22].Set_UV(.25f, twoThird);
	this->poVerts[23].Set_UV(.5f, twoThird);

}

void Model_Skybox::SetNightUVs()
{
	float third = 1.0f / 3.0f;
	float twoThird = third * 2.0f;

	// Front
	this->poVerts[0].Set_UV(0.25f, 0.0f);
	this->poVerts[1].Set_UV(0.25f, third);
	this->poVerts[2].Set_UV(0.5f,  third);
	this->poVerts[3].Set_UV(0.5f,  0.0f);

	// Back
	this->poVerts[4].Set_UV(0.25f, 1.0f);
	this->poVerts[5].Set_UV(0.25f, twoThird);
	this->poVerts[6].Set_UV(0.5f,  twoThird);
	this->poVerts[7].Set_UV(0.5f,  1.0f);

	// Left
	this->poVerts[8].Set_UV( 0.0f,  twoThird);
	this->poVerts[9].Set_UV( 0.0f,  third);
	this->poVerts[10].Set_UV(0.25f, third);
	this->poVerts[11].Set_UV(0.25f, twoThird);

	// Right
	this->poVerts[12].Set_UV(0.75f, third);
	this->poVerts[13].Set_UV(0.75f, twoThird);
	this->poVerts[14].Set_UV(0.5f,  twoThird);
	this->poVerts[15].Set_UV(0.5f,  third);

	// Top
	this->poVerts[16].Set_UV(0.5f,  twoThird);
	this->poVerts[17].Set_UV(0.25f, twoThird);
	this->poVerts[18].Set_UV(0.25f, third);
	this->poVerts[19].Set_UV(0.5f,  third);

	// Bottom
	this->poVerts[20].Set_UV(1.0f,  twoThird);
	this->poVerts[21].Set_UV(0.75f, twoThird);
	this->poVerts[22].Set_UV(0.75f, third);
	this->poVerts[23].Set_UV(1.0f,  third);

}

