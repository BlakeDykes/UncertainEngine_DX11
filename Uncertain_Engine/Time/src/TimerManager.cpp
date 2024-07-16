#include "TimerManager.h"
#include "DLinkManager.h"
#include "TimerEvent.h"

using namespace ThreadFramework;

namespace Uncertain
{
	TimerManager::TimerManager(int initialNumReserved, int deltaGrow)
		:ManagerBase(*new DLinkManager(), *new DLinkManager()), FirstRequest(_Time::Duration::ZERO), ZeroSet(false)
	{
		this->mTimer.Tic();
		this->SetReserve(initialNumReserved, deltaGrow);
	}

	TimerManager::~TimerManager()
	{
		this->BaseClear();
	}

	void TimerManager::Update(std::atomic<bool>& quitFlag)
	{
		this->curTime = _Time::Quotient(this->mTimer.Toc(), _Time(_Time::Duration::ONE_MILLISECOND));

		TimerEvent* pCur = (TimerEvent*)this->poActive->GetFirst();

		while (quitFlag.load() != true && pCur != nullptr)
		{
			if (this->curTime > pCur->GetTriggerTime())
			{
				// execute command and mark for delete
				pCur->Trigger();
			}

			pCur = (TimerEvent*)pCur->GetNext();
		}

		if (quitFlag.load() != true)
		{
			this->RemoveTriggered();
		}
	}

	_Time TimerManager::GetTime()
	{
		if (ZeroSet)
		{
			return this->mTimer.Toc() - FirstRequest;
		}
		else
		{
			this->ZeroSet = true;
			this->FirstRequest = mTimer.Toc();
			return _Time(_Time(_Time::Duration::ZERO));
		}
	}

	_Time TimerManager::GetTotalTime()
	{
		return this->mTimer.Toc();
	}

	TimerEvent* TimerManager::Add(Command& pCMD, _Time timeInitiated, int msToTrigger)
	{
		_Time currentTime = mTimer.Toc();

		int triggerTime = _Time::Quotient((currentTime + (currentTime - timeInitiated)), _Time(_Time::Duration::ONE_MILLISECOND)) + msToTrigger;

		TimerEvent* pNode = (TimerEvent*)this->BaseAdd(triggerTime);

		pNode->Set(pCMD, triggerTime);

		return pNode;
	}

	void TimerManager::Remove(TimerEvent& pEvent)
	{
		this->BaseRemove(pEvent);
	}

	void TimerManager::RemoveTriggered()
	{
		TimerEvent* pCur = (TimerEvent*)this->poActive->GetFirst();
		TimerEvent* pTemp;

		while (pCur != nullptr && pCur->GetDeleteMark() == true)
		{
			pTemp = (TimerEvent*)pCur->GetNext();
			BaseRemove(*pCur);
			pCur = pTemp;
		}
	}

	NodeBase* TimerManager::CreateNode()
	{
		return new TimerEvent();
	}

}
