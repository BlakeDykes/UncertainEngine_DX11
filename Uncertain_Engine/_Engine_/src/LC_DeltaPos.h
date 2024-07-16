#pragma once

#include "LightController.h"
#include "SceneLightObject.h"
#include "Vec3.h"

struct LC_DeltaPos : public LightCommand
{
	DeltaMod* pDeltaPos;

	LC_DeltaPos(const Vec3& deltaPos, FrameModifier::ModType mType);

	void Update(LightController::ReflectionController* pReflection);

protected:
	virtual void privUpdate() = 0;
	virtual void privUpdateReflection(LightController::ReflectionController* pReflection) = 0;
};

struct LC_DeltaPos_Point : public LC_DeltaPos
{
	PhongADS_Point* pLight;

	LC_DeltaPos_Point(PhongADS_Point* pLight, const Vec3& deltaPos, FrameModifier::ModType mType);

protected:
	virtual void privUpdate() override;
	virtual void privUpdateReflection(LightController::ReflectionController* pReflection) override;
};
struct LC_DeltaPos_Spot : public LC_DeltaPos
{
	PhongADS_Spot* pLight;

	LC_DeltaPos_Spot(PhongADS_Spot* light, const Vec3& deltaPos, FrameModifier::ModType mType);

protected:
	virtual void privUpdate() override;
	virtual void privUpdateReflection(LightController::ReflectionController* pReflection) override;
};
