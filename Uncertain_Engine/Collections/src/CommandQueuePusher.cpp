#include "CommandQueuePusher.h"
#include "CommandQueue.h"
#include "Command.h"

namespace Uncertain
{
	CommandQueuePusher::CommandQueuePusher(CommandQueue* queue)
		: pQueue(queue)
	{
	}

	void CommandQueuePusher::Push(Command* c)
	{
		std::lock_guard<std::mutex> m(mtx);

		if (pQueue)
		{
			pQueue->PushBack(c);
		}
	}

	CommandQueuePusher* CommandQueuePusher::GetPusher()
	{
		return nullptr;
	}

}

