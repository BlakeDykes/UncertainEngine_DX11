#include "ReflectionPlane.h"
#include "ModelManager.h"
#include "GraphicsObject.h"

ReflectionPlane::ReflectionPlane()
	: poMirror(nullptr), poReflectionMat(nullptr)
{
}

ReflectionPlane::~ReflectionPlane()
{
	delete poReflectionMat;
	delete poMirror;
}

void ReflectionPlane::Set(GameObject& mirrorObj)
{
	poMirror = &mirrorObj;

	mirrorObj.Update();

	Mat4* world = mirrorObj.GetWorld();

	Vec4 N = world->get(Mat4::Row::i1);
	Vec4 p = world->get(Mat4::Row::i3);
	float d = -p.dot(N);
	Mat4 NTN(N[x] * N, N[y] * N, N[z] * N, d * N);
	NTN[m3] = 0;
	NTN[m7] = 0;
	NTN[m11] = 0;
	poReflectionMat = new Mat4(Mat4(Mat4::IDENTITY_MAT4) - 2 * NTN);
	(*poReflectionMat)[m15] = 1.0f;
}

void ReflectionPlane::Render(ID3D11DeviceContext* context)
{
	this->poMirror->Render(context);
}

bool ReflectionPlane::Compare(NodeBase& pNode)
{
	return this->GetHandle() == static_cast<ReflectionPlane*>(&pNode)->GetHandle();
}

const char* ReflectionPlane::GetName()
{
	return "Reflection Plane";
}

void ReflectionPlane::Print()
{
}

void ReflectionPlane::Update()
{
	this->poMirror->Update();
}
