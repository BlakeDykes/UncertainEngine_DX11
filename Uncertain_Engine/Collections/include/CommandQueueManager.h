#pragma once

#include "ManagerBase.h"
#include "CommandQueue.h"

namespace Uncertain
{
	class Command;

	class CommandQueueManager : public ManagerBase
	{
	public:
		CommandQueueManager() = delete;
		CommandQueueManager(const CommandQueueManager&) = delete;
		CommandQueueManager& operator =(const CommandQueueManager&) = delete;
		virtual ~CommandQueueManager();

		static void Create(int initialNumReserved = 5, int deltaGrow = 2);
		static void Destroy();

		static CommandQueue* Add(ComQueueName queueName);
		static CommandQueue* Find(NodeBase::Handle id);
		static CommandQueue* Find(ComQueueName queueName);
		static void Remove(CommandQueue& pQueue);

		static bool SendData(Command* pCommand);
		static bool SendData(NodeBase::Handle id, Command* pCommand);
		static bool SendData(ComQueueName qName, Command* pCommand);

		virtual NodeBase* CreateNode() override;

	private:
		CommandQueueManager(int initialReserved, int deltaGrow);
		static CommandQueueManager* GetInstance();
		void Clear();

		static CommandQueueManager* instance;
		CommandQueue* poCompNode;
	};
}
