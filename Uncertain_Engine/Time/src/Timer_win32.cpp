//--------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "_Timer.h"

//---------------------------------------------------------------------------
// NATIVE IMPLEMENTATION:
//---------------------------------------------------------------------------

namespace Uncertain
{
	const _Time _Timer::privGetSystemTime()
	{
		const static _Time oneSecond(_Time::Duration::ONE_SECOND);

		_Time systemTime;

		LARGE_INTEGER counterStruct;
		LARGE_INTEGER frequencyStruct;

		if (0 != QueryPerformanceCounter(&counterStruct) && 0 != QueryPerformanceFrequency(&frequencyStruct))
		{
			const LONGLONG counter = counterStruct.QuadPart;
			const LONGLONG frequency = frequencyStruct.QuadPart;

			if (counter >= 0 && 0 != frequency)
			{
				const int numSeconds = static_cast<int>(counter / frequency);
				const float remainder = static_cast<float>(counter % frequency);

				systemTime = numSeconds * oneSecond;
				systemTime += (remainder / (float)frequency) * oneSecond;
			}
		}

		return(systemTime);
	}
}

// ---  End of File ---
