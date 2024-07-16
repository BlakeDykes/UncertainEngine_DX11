#include "Model_AudioSpectrum.h"

Model_AudioSpectrum::Model_AudioSpectrum(ID3D11Device* dev, UINT sampleCount, float len, float yBase)
	: Model(dev, AudioSpectrum)
{
	float xIncrement = (len / sampleCount) * -1.0f;
	float currentX = len * 0.5f;
	float topY = (yBase * 0.5f);
	float botY = topY * -1.0f;

	this->numVerts = sampleCount * 2;
	this->poAVerts = new AudioVertex[numVerts];

	this->numTris = numVerts - 2;
	this->poTris = new TriangleByIndex[numTris];

	unsigned int specIndex = 0;
	for (int i = 0; 
		i < numVerts-2; 
		i+=2, ++specIndex
		)
	{
		poAVerts[i].Set(	   currentX, botY,  0.0f, -1, 0,         0.0f, 0.0f, -1.0f);
		poAVerts[i + 1].Set(   currentX, topY,  0.0f,  1, specIndex, 0.0f, 0.0f, -1.0f);

		poTris[i].Set(    i + 2, i + 1, i);
		poTris[i + 1].Set(i + 3, i + 1, i + 2);

		currentX += xIncrement;
	}

	poAVerts[numVerts - 2].Set(len, 0.0f,  botY, -1, 0,                      0.0f, 0.0f, -1.0f);
	poAVerts[numVerts - 1].Set(len, yBase, topY,  1, specIndex, 0.0f, 0.0f, -1.0f);

	this->poMeshes = new Mesh(numTris);
	protLoadDataToGPU();
}

void Model_AudioSpectrum::SetToCurrent(ID3D11DeviceContext* context)
{
	UINT stride = sizeof(AudioVertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mpoVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(mpoIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model_AudioSpectrum::protLoadDataToGPU()
{
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(AudioVertex) * numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = poAVerts;
	HRESULT hr = mDevice->CreateBuffer(&bd, &InitData, &mpoVertexBuffer);
	assert(SUCCEEDED(hr));

	// Index buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(TriangleByIndex) * numTris;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = poTris;
	hr = mDevice->CreateBuffer(&bd, &InitData, &mpoIndexBuffer);
	assert(SUCCEEDED(hr));
}
