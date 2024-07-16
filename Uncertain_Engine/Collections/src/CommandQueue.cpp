#include "CommandQueue.h"

namespace Uncertain
{
	CommandQueue::CommandQueue()
		: QName(ComQueueName::UNINITIALIZED),
		poPusher(new CommandQueuePusher(this)),
		front(Command_DATA_SIZE),
		back(Command_DATA_SIZE),
		empty(true),
		full(false)
	{
		for (int i = 0; i < Command_DATA_SIZE; i++)
		{
			this->data[i] = 0;
		}
	}

	CommandQueue::~CommandQueue()
	{
		delete this->poPusher;
	}

	bool CommandQueue::PushBack(Command* val)
	{
		this->mtx.lock();

		bool status = true;

		if (this->front != this->back || this->empty == true)
		{
			this->data[this->back.GetIndex()] = val;
			this->back++;

			this->empty = false;

			if (this->front == this->back)
			{
				this->full = true;
			}
		}
		else
		{
			status = false;
		}

		this->mtx.unlock();

		return status;
	}

	bool CommandQueue::PopFront(Command*& val)
	{
		this->mtx.lock();

		bool status = true;

		if (this->front != this->back || this->full == true)
		{
			val = this->data[this->front.GetIndex()];

			this->front++;

			this->full = false;

			if (this->front == this->back)
			{
				this->empty = true;
			}
		}
		else
		{
			status = false;
		}

		this->mtx.unlock();

		return status;
	}

	void CommandQueue::Set(ComQueueName threadName)
	{
		this->QName = threadName;
	}
	void CommandQueue::Wash()
	{
		this->mtx.lock();
		DLink::Wash();

		this->QName = ComQueueName::UNINITIALIZED;
		front.SetIndex(0);
		back.SetIndex(0);
		empty = true;
		full = false;

		for (int i = 0; i < Command_DATA_SIZE; i++)
		{
			this->data[i] = 0;
		}

		this->mtx.unlock();
	}

	bool CommandQueue::Compare(NodeBase& pNode)
	{
		CommandQueue* pComp = (CommandQueue*)&pNode;

		return this->QName == pComp->GetQueueName();
	}
}
