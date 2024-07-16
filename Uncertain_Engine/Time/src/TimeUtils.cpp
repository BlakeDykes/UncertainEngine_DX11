#include "TimeUtils.h"
#include "_Time.h"
#include "_Timer.h"

using namespace ThreadFramework;

namespace Uncertain
{
	bool TimeUtils::WaitForTime(_Timer& t, _Time triggerTime, std::atomic<bool>& quitFlag, bool print)
	{
		_Time deltaTime(_Time::Duration::ZERO);

		if (deltaTime != triggerTime)
		{
			while (!_Time::Quotient(deltaTime, triggerTime) && quitFlag.load() == false)
			{
				deltaTime = t.Toc();
			}
		}

		if (print)
		{
			Debug::out("Current time - %d seconds\n", _Time::Quotient(deltaTime, _Time(_Time::Duration::ONE_SECOND)));
		}

		return true;
	}

	bool TimeUtils::WaitForTime(_Timer& t, _Time triggerTime)
	{
		_Time deltaTime(_Time::Duration::ZERO);

		if (deltaTime != triggerTime)
		{
			while (!_Time::Quotient(deltaTime, triggerTime))
			{
				deltaTime = t.Toc();
			}
		}

		return true;
	}
}