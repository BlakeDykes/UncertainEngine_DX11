#include "RTPCC_LightParam.h"
#include "SceneLightManager.h"
#include "_Helpers.h"

Uncertain::RTPCC_LightParam::RTPCC_LightParam(LPController* paramController, const Vec3& min, const Vec3& max)
	: RTPCC_Base(min, max), pLPC(paramController)
{
}

Uncertain::RTPCC_LightParam::~RTPCC_LightParam()
{
}

Uncertain::RTPCC_LP_Dir::RTPCC_LP_Dir(PhongADS_Directional* light, LightParameters param, const Vec3& min, const Vec3& max)
	: RTPCC_LightParam(LightParamController::GetParamController(param), min, max), pLight(light)
{
}

void Uncertain::RTPCC_LP_Dir::protExecute(const AkRtpcValue& val, const float& lerpProgress)
{
	AZUL_UNUSED_VAR(lerpProgress);
	AZUL_UNUSED_VAR(val);

	//DBOUT("Value = " << lerpProgress << " | poCurrent = (" << poCurrent->x() << ", " << poCurrent->y() << ", " << poCurrent->z() << ")\n");

	pLPC->Update(pLight, (const Vec4&)*poCurrent);
}

Uncertain::RTPCC_LP_Point::RTPCC_LP_Point(PhongADS_Point* light, LightParameters param, const Vec3& min, const Vec3& max)
	: RTPCC_LightParam(LightParamController::GetParamController(param), min, max), pLight(light)
{
}

void Uncertain::RTPCC_LP_Point::protExecute(const AkRtpcValue& val, const float& lerpProgress)
{
	AZUL_UNUSED_VAR(lerpProgress);
	AZUL_UNUSED_VAR(val);

	pLPC->Update(pLight, (const Vec4&)*poCurrent);
}

Uncertain::RTPCC_LP_Spot::RTPCC_LP_Spot(PhongADS_Spot* light, LightParameters param, const Vec3& min, const Vec3& max)
	: RTPCC_LightParam(LightParamController::GetParamController(param), min, max), pLight(light)
{
}

void Uncertain::RTPCC_LP_Spot::protExecute(const AkRtpcValue& val, const float& lerpProgress)
{
	AZUL_UNUSED_VAR(lerpProgress);
	AZUL_UNUSED_VAR(val);
	pLPC->Update(pLight, (const Vec4&)*poCurrent);
}
