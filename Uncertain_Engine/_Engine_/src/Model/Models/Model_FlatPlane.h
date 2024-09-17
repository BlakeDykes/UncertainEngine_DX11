#pragma once

#include "Model/Model.h"

class Model_FlatPlane : public Model
{
public:
	Model_FlatPlane(ID3D11Device* dev, float len, float urep, float vrep);
};