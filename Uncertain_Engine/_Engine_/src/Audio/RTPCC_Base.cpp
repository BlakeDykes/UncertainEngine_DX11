#include "RTPCC_Base.h"
#include "Vec3App.h"

Uncertain::RTPCC_Base::~RTPCC_Base()
{
	delete this->poCurrent;
	delete this->poMax;
	delete this->poMin;
}

void Uncertain::RTPCC_Base::Execute(AkRtpcID id, const AkRtpcValue& val, const float& lerpProgress)
{
	AZUL_UNUSED_VAR(id);
	Vec3App::Lerp(*poCurrent, *poMin, *poMax, lerpProgress);

	protExecute(val, lerpProgress);
}

void Uncertain::RTPCC_GameObj::Execute(AkRtpcID id, const AkRtpcValue& val, const float& lerpProgress)
{
	AK::SoundEngine::SetRTPCValue(id, val, GameObjID); 
	Vec3App::Lerp(*poCurrent, *poMin, *poMax, lerpProgress);

	protExecute(val, lerpProgress);
}
