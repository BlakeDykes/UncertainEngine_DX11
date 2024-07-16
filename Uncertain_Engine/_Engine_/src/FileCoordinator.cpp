#include "FileCoordinator.h"
#include "FileThread.h"
#include "Command.h"

namespace Uncertain
{
	FileCoordinator::FileCoordinator(FileThread& f, ComQueueName queueName)
		: QueueThreadBase(queueName),
		poBufferA(new Buffer(FileCoordinator::BUFFER_SIZE)),
		poBufferB(new Buffer(FileCoordinator::BUFFER_SIZE)),
		pPullBuffer(poBufferA),
		pPushBuffer(poBufferB),
		InProgressTransfers(),
		FileThreadRef(f),
		FileToCoordComplete()
	{
	}

	FileCoordinator::~FileCoordinator()
	{

		for (auto i = InProgressTransfers.begin(); i != InProgressTransfers.end(); ++i)
		{
			if ((*i).valid())
			{
				(*i).get();
			}
		}

		delete poBufferA;
		delete poBufferB;
	}

	void FileCoordinator::Load_Async(const char* const fileName)
	{
		std::promise<Buffer*&> pr();
	}

	void FileCoordinator::ThreadMain(PromiseFlag* readyFlag)
	{
		while (!ShouldShutdown())
		{
			PullFileBufferIfReady();

			SwapBuffersIfReady();

			ProcessQueue();
		}
	}

	void FileCoordinator::PullFileBufferIfReady()
	{
		//if (!this->fFileTransfer.valid())
		//{
		//	fFileTransfer = FileThreadRef.GetTransferFuture();
		//}

		//if (this->pPullBuffer->GetStatus() == Buffer::Status::Empty
		//	&& this->fFileTransfer.wait_for(0s) == std::future_status::ready)
		//{
		//	Buffer* pFile = fFileTransfer.get();

		//	if (pFile)
		//	{
		//		/*size_t transferSize = */pFile->TransferTo(this->pPullBuffer);

		//		//Debug::out("File -> Coord Pull size - %d\n", transferSize);

		//		FileToCoordComplete.cv.notify_one();
		//	}
		//}
	}

	void FileCoordinator::SwapBuffersIfReady()
	{
		if (!pPullBuffer->IsEmpty() && pPushBuffer->IsEmpty())
		{
			Buffer* pTemp = pPullBuffer;
			pPullBuffer = pPushBuffer;
			pPushBuffer = pTemp;

			//Debug::out("Push/Pull queues swapped\n");
		}
	}

	void FileCoordinator::ProcessQueue()
	{
		Command* pCom = nullptr;

		if (!pPushBuffer->IsEmpty() && this->InQueue->PopFront(pCom))
		{
			pCom->Execute();

			while (this->pPushBuffer->GetCurrSize() > 0 && this->InQueue->PopFront(pCom))
			{
				pCom->Execute();
			}

			//if (pPushBuffer->IsEmpty())
			//{
				//Debug::out("Coord push buffer -> Wave complete complete\n");
			//}
		}
	}

	void FileCoordinator::PushTo(Buffer* pToBuffer)
	{
		this->pPushBuffer->TransferTo(pToBuffer);
	}


}