#include "RTPCLerp.h"
#include "_Helpers.h"

Uncertain::RTPCLerp::RTPCLerp(AkUniqueID rtpcID)
	: ElapsedTime(_Time::Duration::ZERO),
	TotalTime(_Time::Duration::ZERO), Timer()
{
	GetRTPCRange(rtpcID, MinVal, MaxVal);
	StartVal = 0.0f;
	EndVal = 0.0f;
}

void Uncertain::RTPCLerp::Set(float from, float to, _Time totalTime)
{
	StartVal = from;
	EndVal = to;
	ElapsedTime = _Time(_Time::Duration::ZERO);
	TotalTime = totalTime;

	Timer.Tic();
}

void Uncertain::RTPCLerp::SetCurrent(float currentVal)
{
	if (StartVal < currentVal
		&& currentVal < EndVal
		&& ElapsedTime < TotalTime)
	{
		Set(currentVal, EndVal, TotalTime - ElapsedTime);
	}
	else
	{
		EndVal = currentVal;
	}
}

AkRtpcValue Uncertain::RTPCLerp::GetLerpPosition(float& tOut)
{
	ElapsedTime += Timer.Toc();

	// /0 gaurd
	if (TotalTime == _Time(_Time::Duration::ZERO))
	{
		tOut = (StartVal - MinVal) / (MaxVal - MinVal);
		return StartVal;
	}
	// Lerp finished
	if (ElapsedTime >= TotalTime)
	{
		tOut = (EndVal - MinVal) / (MaxVal - MinVal);
		return EndVal;
	}

	tOut = ElapsedTime / TotalTime;

	//DBOUT(f << "Elapsed = " << _Time::Quotient(ElapsedTime, _Time(_Time::Duration::ONE_MILLISECOND)) << "ms | Total = " << _Time::Quotient(TotalTime, _Time(_Time::Duration::ONE_MILLISECOND)) << "ms | StartV = " << StartVal << " | EndV = " << EndVal << "\n");

	AkRtpcValue value = (StartVal * (1.0f - tOut) + (EndVal * tOut));

	value = value > MaxVal ? MaxVal : (value < MinVal ? MinVal : value);

	tOut = (value - MinVal) / (MaxVal - MinVal);
	
	Timer.Tic();

	return value;
}
