#pragma once

#include "Model.h"

enum class SkyboxTextures
{
	Daylight,
	Night
};
class Model_Skybox : public Model
{
public:
	Model_Skybox(ID3D11Device* dev, SkyboxTextures texName, float scale = 1.0f);

private:
	void SetDaylightUVs();
	void SetNightUVs();
};