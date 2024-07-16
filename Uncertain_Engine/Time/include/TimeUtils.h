#ifndef TIME_UTILS_H
#define TIME_UTILS_H

namespace Uncertain
{
	class _Time;
	class _Timer;

	class TimeUtils
	{
	public:
		static bool WaitForTime(_Timer& t, _Time triggerTime, std::atomic<bool>& quitFlag, bool print = true);
		static bool WaitForTime(_Timer& t, _Time triggerTime);
	};
}

#endif // !TIME_UTILS_H


