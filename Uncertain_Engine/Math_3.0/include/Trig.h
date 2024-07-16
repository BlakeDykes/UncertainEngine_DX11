#pragma once

namespace Uncertain
{
	class Trig
	{
	public:
		// Do your magic here
		static float cos(const float x);
		static float sin(const float x);
		static float tan(const float x);
		static float atan(const float x);
		static float atan2(const float x, const float y);
		static float acos(const float x);
		static float asin(const float x);
		static void cossin(float& cosOut, float& sinOut, float x);
		static float rsqrt(const float x);
		static float sqrt(const float val);


	private:
		struct TrigVect
		{
			union
			{
				__m128 _mv;

				struct
				{
					float x;
					float y;
					float z;
					float w;
				};
			};
		};
	};
}

//--- End of File ---
