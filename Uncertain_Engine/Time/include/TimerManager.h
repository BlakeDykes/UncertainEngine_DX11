#ifndef TIMER_MANAGER_H
#define TIMER_MANAGER_H

#include "ManagerBase.h"
#include "Command.h"
#include "_Time.h"
#include "_Timer.h"

namespace Uncertain
{
	class TimerEvent;

	class TimerManager : public ManagerBase
	{
	public:

		TimerManager(int initialNumReserved = 5, int deltaGrow = 2);
		TimerManager& operator = (const TimerManager&) = delete;
		TimerManager(const TimerManager&) = delete;
		virtual ~TimerManager();

		void Update(std::atomic<bool>& quitFlag);
		_Time GetTotalTime();
		_Time GetTime();

		// Todo:: add sort
		TimerEvent* Add(Command& pCMD, _Time timeInitiated, int msToTrigger);
		void Remove(TimerEvent& pEvent);
		void RemoveTriggered();

		virtual NodeBase* CreateNode() override;


	private:
		_Timer mTimer;
		_Time FirstRequest;
		int curTime;
		bool ZeroSet;
	};
}

#endif