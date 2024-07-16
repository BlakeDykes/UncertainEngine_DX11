#include "RTPCObject.h"
#include "RTPCC_Base.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

Uncertain::RTPCObject::RTPCObject(AkUniqueID rtpcID)
	: Value(0.0f), Subs(), poLerp(new RTPCLerp(rtpcID)), LerpProgress(0.0f)
{
}

Uncertain::RTPCObject::~RTPCObject()
{
	delete poLerp;

	for (auto it = Subs.begin(); it != Subs.end(); ++it)
	{
		delete (*it);
	}
}

void Uncertain::RTPCObject::Set(AkRtpcValue val)
{
	Value = val;
	poLerp->SetCurrent(val);
}

void Uncertain::RTPCObject::LerpToVal(float to, _Time lerpTotalTime)
{
	this->poLerp->Set(Value, to, lerpTotalTime);
}

void Uncertain::RTPCObject::Add(float value)
{
	Value += value;
	poLerp->SetCurrent(Value);
}

void Uncertain::RTPCObject::Reduce(float value)
{
	Value -= value;
	poLerp->SetCurrent(Value);
}

void Uncertain::RTPCObject::Update(const AkRtpcID& rpctID)
{
	Value = poLerp->GetLerpPosition(LerpProgress);

	AK::SoundEngine::SetRTPCValue(rpctID, Value);

	for (auto it = Subs.begin(); it != Subs.end(); ++it)
	{
		(*it)->Execute(rpctID, Value, LerpProgress);
	}
}
