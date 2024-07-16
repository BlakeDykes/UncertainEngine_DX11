#pragma once

#include <AK/SoundEngine/Common/AkTypes.h>
#include "RTPCLerp.h"
#include "AK/SoundEngine/Common/AkTypes.h"
#include <list>

namespace Uncertain
{
	class RTPCC_Base;

	class RTPCObject
	{
	public:
		RTPCObject() = delete;
		RTPCObject(const RTPCObject&) = delete;
		RTPCObject& operator=(const RTPCObject&) = delete;
		~RTPCObject();

		RTPCObject(AkUniqueID rtpcID);

		void Subscribe(RTPCC_Base* pCom) { Subs.push_back(pCom); }
		void Set(AkRtpcValue val);

		void LerpToVal(float to, _Time lerpTotalTime);

		void Add(float value);
		void Reduce(float value);

		void Update(const AkRtpcID& rpctID);

	private:
		AkRtpcValue Value;
		RTPCLerp* poLerp;
		float LerpProgress;
		std::list<RTPCC_Base*> Subs;
	};
}