#ifndef TIMER_EVENT_H
#define TIMER_EVENT_H

#include "DLink.h"

namespace Uncertain
{
	class Command;

	class TimerEvent : public DLink
	{
	public:
		TimerEvent();
		TimerEvent(Command& command, int triggerTime);
		TimerEvent(const TimerEvent&) = delete;
		TimerEvent operator=(const TimerEvent&) = delete;
		virtual ~TimerEvent();

		void Set(Command& command, int triggerTime);
		void Trigger();

		virtual bool Compare(NodeBase& pNode) override;
		virtual void Wash() override;
		virtual PriorityCompResult ComparePriority(int priority) override;

		int GetTriggerTime() const { return this->TimeToTrigger; }
		bool GetDeleteMark() const { return this->DeleteMark; }

		static int INVALID_TIME;

	private:
		Command* pCMD;
		int TimeToTrigger;
		bool DeleteMark;

	};
}


#endif