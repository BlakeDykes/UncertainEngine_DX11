#pragma once

#include "DLink.h"
#include "RTPCManager.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "Vec3.h"

namespace Uncertain
{
	class RTPCC_Base
	{
	public:
		RTPCC_Base() = delete;
		RTPCC_Base(const RTPCC_Base&) = delete;
		RTPCC_Base& operator=(const RTPCC_Base&) = delete;
		virtual ~RTPCC_Base();
		
		RTPCC_Base(const Vec3& min = Vec3(0.0f, 0.0f, 0.0f), const Vec3& max = Vec3(0.0f, 0.0f, 0.0f))
			: poCurrent(new Vec3()), poMin(new Vec3(min)), poMax(new Vec3(max))
		{
		}

		void SetMin(const Vec3& v) { poMin->set(v); }
		void SetMax(const Vec3& v) { poMax->set(v); }

		virtual void Execute(AkRtpcID id, const AkRtpcValue& val, const float& lerpProgress);

	protected:
		virtual void protExecute(const AkRtpcValue& val, const float& lerpProgress) = 0;

		Vec3* poCurrent;
		Vec3* poMin;
		Vec3* poMax;
	};

	class RTPCC_GameObj : public RTPCC_Base
	{
		RTPCC_GameObj(AkGameObjectID gameObjID, const Vec3& min = Vec3(0.0f, 0.0f, 0.0f), const Vec3& max = Vec3(0.0f, 0.0f, 0.0f))
			: RTPCC_Base(min, max), GameObjID(gameObjID)
		{ }

		virtual void Execute(AkRtpcID id, const AkRtpcValue& val, const float& lerpProgress) override;

	protected:
		AkGameObjectID GameObjID;
	};
}
