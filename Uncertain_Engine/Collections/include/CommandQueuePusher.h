#pragma once

namespace Uncertain
{

	class CommandQueue;
	class Command;

	class CommandQueuePusher
	{
	public:
		CommandQueuePusher() = delete;
		CommandQueuePusher(CommandQueue* queue);

		CommandQueuePusher(const CommandQueuePusher& q) = delete;
		CommandQueuePusher& operator=(const CommandQueuePusher& q) = delete;
		~CommandQueuePusher() = default;

		void Push(Command* c);

		CommandQueuePusher* GetPusher();

	private:
		std::mutex mtx;
		CommandQueue* pQueue;
	};
}
