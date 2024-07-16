#pragma once

#include "Model.h"

class Model_NULL : public Model 
{
public:
	Model_NULL() 
		: Model()
	{}

	virtual void SetToCurrent(ID3D11DeviceContext* context) override { AZUL_UNUSED_VAR(context); }
	virtual void Render(ID3D11DeviceContext* context)  override { AZUL_UNUSED_VAR(context); }
	virtual void RenderMesh(ID3D11DeviceContext* context, int meshnum)  override { AZUL_UNUSED_VAR(context); AZUL_UNUSED_VAR(meshnum); }
	virtual int GetMeshCount() override { return 0; }
};