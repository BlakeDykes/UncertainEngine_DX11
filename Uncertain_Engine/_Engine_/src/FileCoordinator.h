#pragma once

#include "ThreadBase.h"
#include "Buffer.h"
#include <vector>

namespace Uncertain
{
	class FileThread;

	class FileCoordinator : public QueueThreadBase
	{
	public:
		static const unsigned int BUFFER_SIZE = 512 * 1024;

		FileCoordinator() = delete;
		FileCoordinator(FileThread& f, ComQueueName queueName);
		virtual ~FileCoordinator();

		void Load_Async(const char* const fileName);

		void PushTo(Buffer* pToBuffer);

	protected:
		virtual void ThreadMain(PromiseFlag* readyFlag) override;

		void PullFileBufferIfReady();
		void SwapBuffersIfReady();
		void ProcessQueue();

		Buffer* poBufferA;
		Buffer* poBufferB;

		Buffer* pPullBuffer;
		Buffer* pPushBuffer;
		std::vector<std::future<Buffer*&>> InProgressTransfers;

		FileThread& FileThreadRef;

		FileToCoordCompleteFlag FileToCoordComplete;
	};
}