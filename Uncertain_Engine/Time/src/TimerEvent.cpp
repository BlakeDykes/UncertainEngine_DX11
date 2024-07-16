#include "TimerEvent.h"
#include "Command.h"

namespace Uncertain
{
	int TimerEvent::INVALID_TIME = -666;

	TimerEvent::TimerEvent()
		: pCMD(nullptr), TimeToTrigger(INVALID_TIME), DeleteMark(false)
	{
	}

	TimerEvent::TimerEvent(Command& command, int triggerTime)
		: pCMD(&command), TimeToTrigger(triggerTime), DeleteMark(false)
	{
	}

	TimerEvent::~TimerEvent()
	{
		if (pCMD)
		{
			delete pCMD;
		}

		this->TimeToTrigger = INVALID_TIME;
	}

	void TimerEvent::Set(Command& command, int triggerTime)
	{
		this->pCMD = &command;
		this->TimeToTrigger = triggerTime;
	}

	void TimerEvent::Trigger()
	{
		this->pCMD->Execute();

		this->DeleteMark = true;
	}

	bool TimerEvent::Compare(NodeBase& pNode)
	{
		TimerEvent* pComp = (TimerEvent*)&pNode;

		return (this->pCMD->GetName() == pComp->pCMD->GetName()
			&& this->TimeToTrigger == pComp->TimeToTrigger);
	}

	void TimerEvent::Wash()
	{
		this->pCMD = nullptr;
		this->TimeToTrigger = INVALID_TIME;
		this->DeleteMark = false;
	}

	DLink::PriorityCompResult TimerEvent::ComparePriority(int priority)
	{
		if (this->TimeToTrigger > priority)
		{
			return DLink::PriorityCompResult::THIS_IS_GREATER_THAN;
		}
		else
		{
			return DLink::PriorityCompResult::THIS_IS_LESS_THAN;
		}
	}
}
