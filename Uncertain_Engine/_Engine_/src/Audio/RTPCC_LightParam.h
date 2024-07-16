#pragma once

#include "RTPCC_Base.h"
#include "LightParamController.h"
#include "Vec4.h"

namespace Uncertain
{
	class RTPCC_LightParam : public RTPCC_Base
	{
	public:
		RTPCC_LightParam(LPController* paramController, const Vec3& min, const Vec3& max);
		virtual ~RTPCC_LightParam();

	protected:
		LPController* pLPC;
	};

	class RTPCC_LP_Dir : public RTPCC_LightParam
	{
	public:
		RTPCC_LP_Dir(PhongADS_Directional* light, LightParameters param, const Vec3& min, const Vec3& max);

	private:
		virtual void protExecute(const AkRtpcValue& val, const float& lerpProgress) override;
		PhongADS_Directional* pLight;
	};

	class RTPCC_LP_Point : public RTPCC_LightParam
	{
	public:
		RTPCC_LP_Point(PhongADS_Point* light, LightParameters param, const Vec3& min, const Vec3& max);
	
	private:
		virtual void protExecute(const AkRtpcValue& val, const float& lerpProgress) override;
		PhongADS_Point* pLight;
	};

	class RTPCC_LP_Spot : public RTPCC_LightParam
	{
	public:
		RTPCC_LP_Spot(PhongADS_Spot* light, LightParameters param, const Vec3& min, const Vec3& max);

	private:
		virtual void protExecute(const AkRtpcValue& val, const float& lerpProgress) override;
		PhongADS_Spot* pLight;
	};
}