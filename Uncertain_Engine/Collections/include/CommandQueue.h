#pragma once

#include <mutex>
#include "CommandIndex.h"
#include "DLink.h"
#include "CommandQueuePusher.h"

namespace Uncertain
{
	class Command;

	class CommandQueue : public DLink
	{
	public:

		static const int Command_DATA_SIZE = 1 << 10; // 32

		CommandQueue();

		CommandQueue(const CommandQueue&) = delete;
		const CommandQueue& operator =(const CommandQueue&) = delete;

		~CommandQueue();

		bool PushBack(Command* val);
		bool PopFront(Command*& val);

		void Set(ComQueueName queueName);
		ComQueueName GetQueueName() const { return this->QName; }
		CommandQueuePusher* GetPusher() { return this->poPusher; }

		virtual const char* GetName() override { return Stringify(QName); }
		virtual void Wash() override;
		virtual bool Compare(NodeBase& pNode) override;

	private:
		Command* data[Command_DATA_SIZE];
		ComQueueName QName;
		
		CommandQueuePusher* poPusher;

		CommandIndex front;
		CommandIndex back;

		bool empty;
		bool full;

		std::mutex mtx;

	};
}
