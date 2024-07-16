#include "LC_DeltaPos.h"

LC_DeltaPos::LC_DeltaPos(const Vec3& deltaPos, FrameModifier::ModType mtype)
	: LightCommand(CommandName::Light_SetDeltaPos, ComQueueName::GAME), 
	pDeltaPos(ModManager::CreateDeltaMod(deltaPos, mtype))
{
}


void LC_DeltaPos::LC_DeltaPos::Update(LightController::ReflectionController* pReflection)
{
	privUpdate();
	privUpdateReflection(pReflection);
}

LC_DeltaPos_Point::LC_DeltaPos_Point(PhongADS_Point* light, const Vec3& deltaPos, FrameModifier::ModType mType)
	: LC_DeltaPos(deltaPos, mType), pLight(light)
{
}

void LC_DeltaPos_Point::privUpdate()
{
	this->pLight->Translate(pDeltaPos->GetDeltaMod());
}

void LC_DeltaPos_Point::privUpdateReflection(LightController::ReflectionController* pReflection)
{
	pReflection->Update(pLight);
}

LC_DeltaPos_Spot::LC_DeltaPos_Spot(PhongADS_Spot* light, const Vec3& deltaPos, FrameModifier::ModType mType)
	: LC_DeltaPos(deltaPos, mType), pLight(light)
{
}

void LC_DeltaPos_Spot::privUpdate()
{
	this->pLight->Translate(pDeltaPos->GetDeltaMod());
}

void LC_DeltaPos_Spot::privUpdateReflection(LightController::ReflectionController* pReflection)
{
	pReflection->Update(pLight);
}
