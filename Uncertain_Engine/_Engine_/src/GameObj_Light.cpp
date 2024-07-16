#include "GameObj_Light.h"
#include "SceneLightObject.h"

GameObj_Light::GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order)
	: GameObject(graphicsObject, name, order), poLightM(nullptr)
{
}

GameObj_Light::GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order, SceneLightObject* sceneLight, PhongADS_Directional* pLight)
	: GameObject(graphicsObject, name, order),
	poLightM(new Dir_Messenger(sceneLight, sceneLight->GetIndex(pLight)))
{
	Vec4 rot = pLight->GetDirection();
	this->poRot->set(Quat::Rot3::XYZ, rot.x(), rot.y(), rot.z());
}

GameObj_Light::GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order, SceneLightObject* sceneLight, PhongADS_Point* pLight)
	: GameObject(graphicsObject, name, order),
	poLightM(new Point_Messenger(sceneLight, sceneLight->GetIndex(pLight)))
{
	this->poTrans->set(pLight->GetPosition());
}

GameObj_Light::GameObj_Light(GraphicsObject* graphicsObject, const char* const name, RenderOrder order, SceneLightObject* sceneLight, PhongADS_Spot* pLight)
	: GameObject(graphicsObject, name, order),
	poLightM(new Spot_Messenger(sceneLight, sceneLight->GetIndex(pLight)))
{
	this->poTrans->set(pLight->GetPosition());
	Vec4 rot = pLight->GetDirection();
	this->poRot->set(Quat::Rot3::XYZ, rot.x(), rot.y(), rot.z());
}

GameObj_Light::~GameObj_Light()
{
	delete poLightM;
}

void GameObj_Light::Update()
{
	GameObject* pParentObj = (GameObject*)this->GetParent();
	Mat4 parentMat;
	if (pParentObj)
	{
		parentMat = *pParentObj->GetWorld();
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

	this->poLightM->SendWorld(localRot, *poRot, *poWorld);
}

void GameObj_Light::Set(SceneLightObject* sceneLight, PhongADS_Directional* pLight)
{
	poLightM = new Dir_Messenger(sceneLight, sceneLight->GetIndex(pLight));
	Vec4 rot = pLight->GetDirection();
	this->poRot->set(Quat::Rot3::XYZ, rot.x(), rot.y(), rot.z());
}

void GameObj_Light::Set(SceneLightObject* sceneLight, PhongADS_Point* pLight)
{
	poLightM = new Point_Messenger(sceneLight, sceneLight->GetIndex(pLight));
	this->poTrans->set(pLight->GetPosition());
}

void GameObj_Light::Set(SceneLightObject* sceneLight, PhongADS_Spot* pLight)
{
	poLightM = new Spot_Messenger(sceneLight, sceneLight->GetIndex(pLight));
	this->poTrans->set(pLight->GetPosition());
	Vec4 rot = pLight->GetDirection();
	this->poRot->set(Quat::Rot3::XYZ, rot.x(), rot.y(), rot.z());
}

GameObj_Light::Dir_Messenger::Dir_Messenger(SceneLightObject* sceneLight, size_t index)
	: LightUpdateMessenger(sceneLight, index)
{
}

void GameObj_Light::Dir_Messenger::SendWorld(const Mat4& localRot, const Quat& thisRot, Mat4& mWorld)
{
	ALIGN_UNUSED_VAR(localRot);
	AZUL_UNUSED_VAR(thisRot);

	this->pSceneLight->UpdateWorld_Dir(LightIndex, mWorld.get(Mat4::Row::i3), mWorld.get(Mat4::Row::i3));
}

GameObj_Light::Point_Messenger::Point_Messenger(SceneLightObject* sceneLight, size_t index)
	: LightUpdateMessenger(sceneLight, index)
{
}

void GameObj_Light::Point_Messenger::SendWorld(const Mat4& localRot, const Quat& thisRot, Mat4& mWorld)
{
	ALIGN_UNUSED_VAR(thisRot);
	ALIGN_UNUSED_VAR(localRot);

	this->pSceneLight->UpdateWorld_Point(LightIndex, mWorld.get(Mat4::Row::i2), mWorld.get(Mat4::Row::i3));
}


GameObj_Light::Spot_Messenger::Spot_Messenger(SceneLightObject* sceneLight, size_t index)
	: LightUpdateMessenger(sceneLight, index)
{
}

void GameObj_Light::Spot_Messenger::SendWorld(const Mat4& localRot, const Quat& thisRot, Mat4& mWorld)
{
	ALIGN_UNUSED_VAR(localRot);
	AZUL_UNUSED_VAR(thisRot);

	this->pSceneLight->UpdateWorld_Dir(LightIndex, mWorld.get(Mat4::Row::i2), mWorld.get(Mat4::Row::i3));
}

