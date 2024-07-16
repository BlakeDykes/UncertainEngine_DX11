#include "ThreadBase.h"
#include "ThreadManager.h"
#include "CommandQueueManager.h"
#include <windows.h>
#include <tchar.h>

namespace Uncertain
{

	ThreadBase::ThreadBase(const char* const pName)
		: BannerBase(pName),
		mThread(),
		fShouldShutdown(ThreadManager::GetShutdownFuture())
	{
	}

	ThreadBase::~ThreadBase()
	{
		if (this->mThread.joinable())
		{
			this->mThread.join();
		}
	}

	void ThreadBase::Launch(Priority p, PromiseFlag* readyFlag)
	{
		this->ThreadPriority = p;

		if (this->mThread.joinable() == false)
		{
			this->mThread = std::thread(std::ref(*this), readyFlag);
		}
		else
		{
			if (readyFlag) readyFlag->Flag();
		}
	}

	bool ThreadBase::ShouldShutdown()
	{
		return fShouldShutdown.wait_for(0s) == std::future_status::ready;
	}

	void ThreadBase::operator()(PromiseFlag* readyFlag)
	{
		START_BANNER;

		ThreadManager::RegisterThreadStart();

		SetPriority(this->ThreadPriority);

		ThreadMain(readyFlag);

		Shutdown();
	}

	void ThreadBase::SetPriority(Priority p)
	{
		DWORD dwError;

		if (!SetThreadPriority(mThread.native_handle(), (int)p))
		{
			dwError = GetLastError();
			_tprintf(TEXT("Failed to set thread priority: (%d)\n"), (int)dwError);

		}
	}

	ThreadBase::Priority ThreadBase::GetPriority()
	{
		return (ThreadBase::Priority)GetThreadPriority(GetCurrentThread());
	}

	void ThreadBase::Shutdown()
	{
		ThreadManager::RegisterThreadEnd();
	}

	QueueThreadBase::QueueThreadBase(ComQueueName name)
		: ThreadBase(Stringify(name)), InQueue(CommandQueueManager::Add(name))
	{
	}

	CommandQueuePusher* QueueThreadBase::GetQueuePusher()
	{
		return InQueue->GetPusher();
	}

}

