#pragma once

#include "DLink.h"
#include "GameObject.h"
#include <vector>

class ReflectionPlane : public DLink
{
public:
	ReflectionPlane();
	ReflectionPlane(const ReflectionPlane&) = delete;
	ReflectionPlane operator=(const ReflectionPlane&) = delete;
	~ReflectionPlane();

	void Set(GameObject& mirrorObj);

	void Update();
	void Render(ID3D11DeviceContext* context);

	const Mat4& GetReflectionMat() { return *poReflectionMat; }
	
	// Inherited via DLink
	bool Compare(NodeBase& pNode) override;

	void Scale(float f) { this->poMirror->Scale(f); }
	void Translate(const Vec3& v) { this->poMirror->Translate(v); }
	void Rotate(const Quat& q) { this->poMirror->Rotate(q); }

	const Mat4* GetWorld() const { return this->poMirror->GetWorld(); }
	void SetDeltaRot(const Vec3& r) { this->poMirror->SetDeltaRot(r); }

private:
	GameObject* poMirror;
	Mat4* poReflectionMat;

	// Inherited via DLink
	const char* GetName() override;
	void Print() override;

};