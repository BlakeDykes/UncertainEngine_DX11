#pragma once

#include <d3d11.h>
#include "Mat4.h"
#include "QuatApp.h"
#include "d3dUtil.h"
#include "Utils.h"
#include "DXApp.h"
#include "PCSNode.h"
#include "_Time.h"
#include "AudioNode.h"

class GraphicsObject;
class ReflectionPlane;

class GameObject : public PCSNode
{
public:
	enum class RenderOrder : BYTE
	{
		Default = 0x0,
		Reflected,
		Deferred,
		ReflectionObject
	};

	enum class Type : BYTE
	{
		Default = 0x0,
		Root,
		Light
	};

protected:
	Mat4* poWorld;
	Vec3* poScale;
	Quat* poRot;
	Vec3* poTrans;
	
	Vec3* poLocalRot;
	Vec3* poLocalTrans;
	DeltaMod* pDeltaPos;
	DeltaMod* pDeltaRot;

	GraphicsObject* pGraphicsObject;
	ReflectionPlane* pReflectionParent;
	RenderOrder ROrder;
	Type ObjectType;
	const char _padding[2] = {0};

public:

	GameObject(GraphicsObject* graphicsObject, const char* const name, RenderOrder order);
	GameObject(GraphicsObject* graphicsObject, const char* const name,
		const Vec3& scale, const Quat& rot, const Vec3& trans, RenderOrder order = RenderOrder::ReflectionObject);

	virtual ~GameObject();

	virtual void Update();
	virtual void Render(ID3D11DeviceContext* context);
	virtual void ReflectWorld(const Mat4& reflectionMatrix);
	
	Mat4* GetWorld() const { return this->poWorld; }
	
	const Vec3& GetScale() const { return *this->poScale; }
	const Quat& GetRot() const { return   *this->poRot; }
	const Vec3& GetTrans() const { return *this->poTrans; }

	void Scale(float f) { *this->poScale *= f; }
	void Translate(const Vec3& v) { *this->poTrans += v; }
	virtual void Rotate(const Quat& q) { *this->poRot = q; }

	virtual void SetDefaultRenderOrder();

	void SetDeltaPos(const Vec3& v, FrameModifier::ModType modType = FrameModifier::ModType::Default);
	void SetDeltaRot(const Vec3& v, FrameModifier::ModType modType = FrameModifier::ModType::Default); 

	RenderOrder GetRenderOrder() const { return this->ROrder; }
	
	void SetReflectionParent(ReflectionPlane* pPlane) { this->pReflectionParent = pPlane; }
	ReflectionPlane* GetReflectionParent() { return this->pReflectionParent; }
	
	virtual void Wash() override;


protected:
	GameObject(Type type, GraphicsObject* graphicsObject, const char* const name, RenderOrder order);

public:

#ifdef CONTROL_OBJ2_DEBUG
	bool bShouldRender_DEBUG;
	void PreventRender_DEBUG();
	void AllowRender_DEBUG();
#endif
};

