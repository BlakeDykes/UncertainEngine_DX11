#pragma once

#include "Model.h"
#include "DirectXTex.h"

static const float TERRAIN_WATER_Y = 10.0f;
static const float TERRAIN_MIN_Y = 10.05f;

class Model_Terrain : public Model
{
public:
	/// <summary>
	/// Create a mesh from a hightmap TGA
	/// </summary>
	/// <param name="dev:">DX11 Device</param>
	/// <param name="handle:">ModelManager handle</param>
	/// <param name="heightMapFP:">file path</param>
	/// <param name="scale:">x and z scale of rects</param>
	/// <param name="maxHeight:">max height of terrain</param>
	/// <param name="yTrans:">Y translation of model</param>
	/// <param name="uRep:">texture repeat U</param>
	/// <param name="vRep:">texture repeat V</param>
	Model_Terrain(ID3D11Device* dev, LPCWSTR heightMapFP,
		float modelLength, float maxHeight, float yTrans, int uRep, int vRep);

	float GetHeight(uint8_t texVal);

private:
	void GenTerrain(const DirectX::Image* heightMap);
	void GenBlendValues();

	int URep;
	int VRep;
	float ModelLength;
	float MaxHeight;
	float YTranslation;

	float SandBreakpoint;
	float GrassBreakpoint;
	float RockBreakpoint;
};