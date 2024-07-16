#pragma once

#include "Model.h"
#include "SA_FFT.h"


class Model_AudioSpectrum : public Model
{
public:
	Model_AudioSpectrum(ID3D11Device* dev, UINT sampleCount, float len, float yScale);

	virtual void SetToCurrent(ID3D11DeviceContext* context);

private:
	virtual void protLoadDataToGPU() override;
	AudioVertex* poAVerts;
};
