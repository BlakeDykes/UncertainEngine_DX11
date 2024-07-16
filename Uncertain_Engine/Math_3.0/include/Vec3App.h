#pragma once

#include "Vec3.h"

namespace Uncertain
{
	class Vec3App
	{

	public:
		static void Lerp(Vec3 &out, const Vec3 &a, const Vec3 &b, const float t);
		static void LerpArray(Vec3 *out, const Vec3 *a, const Vec3 *b, const float t, const int numVects);
	};
}
