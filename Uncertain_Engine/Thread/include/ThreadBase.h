#pragma once

#include "ThreadFlags.h"
#include "CommandQueue.h"

namespace Uncertain
{

	class ThreadBase : public BannerBase
	{

	public:
		enum class Priority : int
		{
			LOWEST			 = THREAD_PRIORITY_LOWEST,
			BELOW_NORMAL	 = THREAD_PRIORITY_BELOW_NORMAL,
			NORMAL			 = THREAD_PRIORITY_NORMAL,
			ABOVE_NORMAL	 = THREAD_PRIORITY_ABOVE_NORMAL,
			HIGHEST			 = THREAD_PRIORITY_HIGHEST,
			TIME_CRITICAL	 = THREAD_PRIORITY_TIME_CRITICAL,
			IDLE		     = THREAD_PRIORITY_IDLE,
			BACKGROUND_BEGIN = THREAD_MODE_BACKGROUND_BEGIN,
			BACKGROUND_END	 = THREAD_MODE_BACKGROUND_END,
		};


		ThreadBase() = delete;
		ThreadBase(const char* const pName);

		ThreadBase(const ThreadBase&) = delete;
		ThreadBase& operator = (const ThreadBase&) = delete;
		virtual ~ThreadBase();

		virtual void Launch(Priority p = Priority::NORMAL, PromiseFlag* readyFlag = nullptr);
		virtual bool ShouldShutdown();

		void operator()(PromiseFlag* readyFlag);

		void SetPriority(Priority p);
		Priority GetPriority();

	protected:
		virtual void ThreadMain(PromiseFlag* readyFlag) = 0;
		virtual void Shutdown();

		std::thread mThread;
		std::future<void>& fShouldShutdown;
		Priority ThreadPriority;
	};

	class QueueThreadBase : public ThreadBase
	{
	public:
		QueueThreadBase(ComQueueName name);

		CommandQueuePusher* GetQueuePusher();
	protected:
		CommandQueue* InQueue;

	};
}
