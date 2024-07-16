//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include <math.h>    // <--- ONLY location for <math.h> allowed
#include "Trig.h"

namespace Uncertain
{

	float Trig::cos(const float x)
	{
		return std::cosf(x);
	}

	float Trig::sin(const float x)
	{
		return std::sinf(x);
	}

	float Trig::tan(const float x)
	{
		return std::tanf(x);
	}

	float Trig::atan(const float x)
	{
		return std::atanf(x);
	}

	float Trig::atan2(const float x, const float y)
	{
		return std::atan2f(x, y);
	}

	float Trig::acos(const float x)
	{
		return std::acosf(x);
	}

	float Trig::asin(const float x)
	{
		return std::asinf(x);
	}

	void Trig::cossin(float& cosOut, float& sinOut, float x)
	{
		cosOut = Trig::cos(x);
		sinOut = Trig::sin(x);
	}

	float Trig::rsqrt(const float x)
	{
		TrigVect _x;
		TrigVect _ret;

		_x._mv = _mm_set1_ps(x);

		_ret._mv = _mm_rsqrt_ps(_x._mv);

		return _ret.x;
	}

	float Trig::sqrt(const float val)
	{
		TrigVect _x;
		TrigVect _ret;

		_x._mv = _mm_set1_ps(val);

		_ret._mv = _mm_sqrt_ps(_x._mv);

		return _ret.x;
	}

}

//--- End of File ---
