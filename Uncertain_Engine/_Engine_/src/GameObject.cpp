#include "GameObject.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "GraphicsObject.h"
#include "GraphicsObjectManager.h"

GameObject::GameObject(GraphicsObject* graphicsObject, const char* const name, RenderOrder order)
	: PCSNode(name),
	poWorld(new Mat4(Mat4::IDENTITY_MAT4)),
	poScale(new Vec3(1.0f, 1.0f, 1.0f)),
	poRot(new Quat(Quat::Special::Identity)),
	poTrans(new Vec3(0.0f, 0.0f, 0.0f)), 
	poLocalRot(new Vec3(0.0f, 0.0f, 0.0f)),
	poLocalTrans(new Vec3(0.0f, 0.0f, 0.0f)),
	pDeltaRot(ModManager::GetNull()),
	pDeltaPos(ModManager::GetNull()),
	pGraphicsObject(graphicsObject),
	pReflectionParent(nullptr),
	ROrder(order),
	ObjectType(Type::Default)
{
#ifdef CONTROL_OBJ2_DEBUG
	bShouldRender_DEBUG = true;
#endif // CONTROL_OBJ2_DEBUG

}

GameObject::GameObject(GraphicsObject* graphicsObject, const char* const name, const Vec3& scale, const Quat& rot, const Vec3& trans, RenderOrder order)
	: PCSNode(name),
	poWorld(new Mat4(Mat4::IDENTITY_MAT4)),
	poScale(new Vec3(scale)),
	poRot(new Quat(rot)),
	poTrans(new Vec3(trans)),
	poLocalRot(new Vec3(0.0f, 0.0f, 0.0f)),
	poLocalTrans(new Vec3(0.0f, 0.0f, 0.0f)),
	pDeltaRot(ModManager::GetNull()),
	pDeltaPos(ModManager::GetNull()),
	pGraphicsObject(graphicsObject),
	pReflectionParent(nullptr),
	ROrder(order),
	ObjectType(Type::Default)
{
#ifdef CONTROL_OBJ2_DEBUG
	bShouldRender_DEBUG = true;
#endif // CONTROL_OBJ2_DEBUG

}
GameObject::~GameObject()
{
	delete this->poLocalRot;
	delete this->poLocalTrans;
	delete this->poTrans;
	delete this->poRot;
	delete this->poScale;
	delete this->poWorld;
}

void GameObject::Update()
{
	GameObject* pParentObj = (GameObject*)this->GetParent();
	Mat4 parentMat;
	if (pParentObj)
	{
		parentMat = *pParentObj->poWorld;
	}
	else
	{
		parentMat = Mat4::Special::Identity;
	}

	Mat4 trans(Mat4::Trans::XYZ, *this->poTrans);

	*poLocalTrans += pDeltaPos->GetDeltaMod();
	Mat4 localTrans(Mat4::Trans::XYZ, *this->poLocalTrans);

	*poLocalRot += pDeltaRot->GetDeltaMod();
	Mat4 localRot(Mat4::Rot3::XYZ, this->poLocalRot->x(), this->poLocalRot->y(), this->poLocalRot->z());
	
	Mat4 scale(Mat4::Scale::XYZ, *this->poScale);

	*this->poWorld = scale * localRot * localTrans * (*this->poRot) * trans * parentMat;
}

void GameObject::Render(ID3D11DeviceContext* context)
{
#ifdef CONTROL_OBJ2_DEBUG
	if (bShouldRender_DEBUG)
	{
		this->pGraphicsObject->Render(context, this);
	}
#else

	this->pGraphicsObject->Render(context, this);

#endif // CONTROL_OBJ2_DEBUG
}

void GameObject::ReflectWorld(const Mat4& reflectionMatrix)
{
	*this->poWorld *= reflectionMatrix;
}

void GameObject::SetDefaultRenderOrder()
{
	this->ROrder = RenderOrder::Default;
	this->pReflectionParent = nullptr;
}

void GameObject::SetDeltaPos(const Vec3& v, FrameModifier::ModType modType)
{
	if (this->pDeltaPos->ModType() == modType)
	{
		pDeltaPos->SetDelta(v);
	}
	else
	{
		ModManager::Exchange(this->pDeltaPos, v, modType);
	}
}

void GameObject::SetDeltaRot(const Vec3& v, FrameModifier::ModType modType)
{
	if (this->pDeltaRot->ModType() == modType)
	{
		pDeltaRot->SetDelta(v);
	}
	else
	{
		ModManager::Exchange(this->pDeltaRot, v, modType);
	}
}


void GameObject::Wash()
{
	this->poWorld->set(Mat4::Special::Identity);
	this->poTrans->set(0.0f, 0.0f, 0.0f);

	if (this->pGraphicsObject)
	{
		GraphicsObjectManager::Remove(*this->pGraphicsObject);
		this->pGraphicsObject = nullptr;
	}
}

GameObject::GameObject(Type type, GraphicsObject* graphicsObject, const char* const name, RenderOrder order)
	: PCSNode(name),
	poWorld(new Mat4(Mat4::IDENTITY_MAT4)),
	poScale(new Vec3(1.0f, 1.0f, 1.0f)),
	poRot(new Quat(Quat::Special::Identity)),
	poTrans(new Vec3(0.0f, 0.0f, 0.0f)),
	poLocalRot(new Vec3(0.0f, 0.0f, 0.0f)),
	poLocalTrans(new Vec3(0.0f, 0.0f, 0.0f)),
	pDeltaRot(ModManager::GetNull()),
	pDeltaPos(ModManager::GetNull()),
	pGraphicsObject(graphicsObject),
	ROrder(order),
	ObjectType(type)
{
#ifdef CONTROL_OBJ2_DEBUG
	bShouldRender_DEBUG = true;
#endif // CONTROL_OBJ2_DEBUG

}

#ifdef CONTROL_OBJ2_DEBUG

void GameObject::PreventRender_DEBUG()
{
	bShouldRender_DEBUG = false;
}

void GameObject::AllowRender_DEBUG()
{
	bShouldRender_DEBUG = true;
}
#endif

