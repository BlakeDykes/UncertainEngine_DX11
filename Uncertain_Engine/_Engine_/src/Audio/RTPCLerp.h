#pragma once

#include "_Timer.h"
#include <AK/SoundEngine/Common/AkTypes.h>

namespace Uncertain
{
	class RTPCLerp
	{
	public:
		RTPCLerp() = delete;
		RTPCLerp(const RTPCLerp&) = delete;
		RTPCLerp& operator=(const RTPCLerp&) = delete;
		virtual ~RTPCLerp() = default;

		RTPCLerp(AkUniqueID rtpcID);
		void Set(float from, float to, _Time totalTime);

		void SetCurrent(float currentVal);

		AkRtpcValue GetLerpPosition(float& tOut);

	private:
		float StartVal;
		float EndVal;
		float MinVal;
		float MaxVal;
		_Time ElapsedTime;
		_Time TotalTime;
		_Timer Timer;
	};
}