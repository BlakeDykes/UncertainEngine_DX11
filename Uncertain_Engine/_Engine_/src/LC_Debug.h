#pragma once

#include "DXApp.h"

#ifdef LIGHT_DEBUG


#include "LightController.h"

class LC_Debug_Dir : public LightCommand
{
public:
	PhongADS_Directional* pLight;
	const char* Id;

	LC_Debug_Dir(PhongADS_Directional* light, const char* const id) : pLight(light), Id(id) {}

	virtual void Update(LightController::ReflectionController* pReflection) override { pLight->DEBUG_Print(Id); pReflection->DEBUG_Print(pLight, Id); }
};

class LC_Debug_Point : public LightCommand
{
public:
	PhongADS_Point* pLight;
	const char* Id;

	LC_Debug_Point(PhongADS_Point* light, const char* const id) : pLight(light), Id(id) {}
	virtual void Update(LightController::ReflectionController* pReflection) override { pLight->DEBUG_Print(Id); pReflection->DEBUG_Print(pLight, Id); }
};

class LC_Debug_Spot : public LightCommand
{
public:
	PhongADS_Spot* pLight;
	const char* Id;

	LC_Debug_Spot(PhongADS_Spot* light, const char* const id) : pLight(light), Id(id) {}
	virtual void Update(LightController::ReflectionController* pReflection) override { pLight->DEBUG_Print(Id); pReflection->DEBUG_Print(pLight, Id); }
};

#endif // DEBUG_LIGHT
