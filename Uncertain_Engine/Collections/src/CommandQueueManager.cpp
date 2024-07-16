#include "CommandQueueManager.h"
#include "DLinkManager.h"
#include "Command.h"

namespace Uncertain
{
	CommandQueueManager* CommandQueueManager::instance = nullptr;

	CommandQueueManager::CommandQueueManager(int initialReserved, int deltaGrow)
		: ManagerBase(*new DLinkManager(), *new DLinkManager())
	{
		this->poCompNode = new CommandQueue();

		this->SetReserve(initialReserved, deltaGrow);
	}

	CommandQueueManager::~CommandQueueManager()
	{
		this->Clear();

		this->poCompNode->Wash();
		delete this->poCompNode;
	}

	void CommandQueueManager::Destroy()
	{
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		delete inst;
	}

	void CommandQueueManager::Create(int initialNumReserved, int deltaGrow)
	{
		assert(instance == nullptr);
		assert(deltaGrow != 0);

		if (instance == nullptr)
		{
			instance = new CommandQueueManager(initialNumReserved, deltaGrow);
		}
	}

	CommandQueue* CommandQueueManager::Add(ComQueueName threadName)
	{
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		CommandQueue* pNode = (CommandQueue*)inst->BaseAdd();

		pNode->Set(threadName);

		return pNode;
	}

	CommandQueue* CommandQueueManager::Find(NodeBase::Handle id)
	{
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		CommandQueue* pNode = (CommandQueue*)inst->BaseFindByHandle(id);

		return pNode;
	}

	CommandQueue* CommandQueueManager::Find(ComQueueName threadName)
	{
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		CommandQueue* pCur = (CommandQueue*)inst->poActive->GetFirst();

		while (pCur)
		{
			if (pCur->GetQueueName() == threadName)
			{
				break;
			}

			pCur = (CommandQueue*)pCur->GetNext();
		}

		return pCur;
	}

	void CommandQueueManager::Remove(CommandQueue& pQueue)
	{
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		inst->BaseRemove(pQueue);
	}

	bool CommandQueueManager::SendData(Command* pCommand)
	{
		assert(pCommand);
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		CommandQueue* pQueue = inst->Find(pCommand->GetExeQueue());
		if (!pQueue)
		{
			return false;
		}

		return pQueue->PushBack(pCommand);
	}

	bool CommandQueueManager::SendData(NodeBase::Handle id, Command* pCommand)
	{
		assert(pCommand);
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		CommandQueue* pQueue = (CommandQueue*)inst->BaseFindByHandle(id);
		if (!pQueue)
		{
			return false;
		}

		return pQueue->PushBack(pCommand);
	}

	bool CommandQueueManager::SendData(ComQueueName qName, Command* pCommand)
	{
		assert(pCommand);
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		CommandQueue* pQueue = inst->Find(qName);
		if (!pQueue)
		{
			return false;
		}

		return pQueue->PushBack(pCommand);
	}

	void CommandQueueManager::Clear()
	{
		CommandQueueManager* inst = CommandQueueManager::GetInstance();

		inst->BaseClear();
	}

	NodeBase* CommandQueueManager::CreateNode()
	{
		return new CommandQueue();
	}

	CommandQueueManager* CommandQueueManager::GetInstance()
	{
		assert(instance != nullptr);
		return instance;
	}
}


