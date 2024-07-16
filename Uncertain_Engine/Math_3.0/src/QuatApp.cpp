//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "MathApp.h"

//----------------------------------------------------------------------------- 
// Mutates result to be a slerp between the source and target by the
// specified factor.
// For a factor of 0.0, result == source.
// For a factor of 1.0, result == target.
//----------------------------------------------------------------------------- 

namespace Uncertain
{
	void QuatApp::Slerp(Quat& result, const Quat& source, const Quat& target, const float slerpFactor)
	{
		float t_cos = source.dot(target);
		
		if (t_cos >= 1.0f)
		{
			result = target;
		}
		else if (t_cos < 0)
		{
			t_cos = -t_cos;
			Quat negTarget = -target;

			float angle = Trig::acos(t_cos);

			result = source * ((Trig::sin((1 - slerpFactor) * angle)) / Trig::sin(angle)) + negTarget * ((Trig::sin(slerpFactor * angle)) / Trig::sin(angle));
		}
		else
		{
			float angle = Trig::acos(t_cos);

			result = source * ((Trig::sin((1 - slerpFactor) * angle)) / Trig::sin(angle)) + target * ((Trig::sin(slerpFactor * angle)) / Trig::sin(angle));
		}
	}

	void QuatApp::SlerpArray(Quat* result, const Quat* source, const Quat* target, const float slerpFactor, const int numQuats)
	{
		for (int i = 0; i < numQuats; i++)
		{
			// bug when this == 1.0f
			float t_cos = source[i].dot(target[i]);

			if (t_cos >= 1.0f)
			{
				result[i] = target[i];
			}
			else if (t_cos > 0.0f && t_cos < 1.0f)
			{
				float angle = Trig::acos(t_cos);

				result[i] = source[i] * ((Trig::sin((1 - slerpFactor) * angle)) / Trig::sin(angle)) + target[i] * ((Trig::sin(slerpFactor * angle)) / Trig::sin(angle));
			}
			else if(t_cos < 0.0f)
			{
				t_cos = -t_cos;
				Quat negTarget = -target[i];

				float angle = Trig::acos(t_cos);

				result[i] = source[i] * ((Trig::sin((1 - slerpFactor) * angle)) / Trig::sin(angle)) + negTarget * ((Trig::sin(slerpFactor * angle)) / Trig::sin(angle));
			}
		}
	}

}

// ---  End of File ---------------
