#include "LightParamController.h"

LPController* LightParamController::GetParamController(LightParameters param)
{
	switch (param)
	{
	case LightParameters::Ambient:				return &Instance()->Ambient;
	case LightParameters::Diffuse:				return &Instance()->Diffuse;
	case LightParameters::Specular:				return &Instance()->Specular;
	case LightParameters::SpecularExponent:		return &Instance()->SpecularExponent;
	case LightParameters::Direction:			return &Instance()->Direction;
	case LightParameters::Position:				return &Instance()->Position;
	case LightParameters::Attenuation:			return &Instance()->Attenuation;
	case LightParameters::Range:				return &Instance()->Range;
	case LightParameters::SpotExponent:			return &Instance()->SpotExponent;
	default:
		assert(false);
		return nullptr;
	}
}

LightParamController* LightParamController::Instance()
{
	static LightParamController inst;
	return &inst;
}

void LPController::Update(PhongADS_Directional* light, const Vec4& v)
{
	if (privUpdate(light, v)) 
	{ 
		LightParamController::UpdateReflection(light); 
	}
}

void LPController::Update(PhongADS_Point* light, const Vec4& v)
{
	if (privUpdate(light, v))
	{
		LightParamController::UpdateReflection(light);
	}
}

void LPController::Update(PhongADS_Spot* light, const Vec4& v)
{
	if (privUpdate(light, v))
	{
		LightParamController::UpdateReflection(light);
	}
}
