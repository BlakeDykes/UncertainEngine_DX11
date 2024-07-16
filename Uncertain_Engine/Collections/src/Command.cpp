#include "Command.h"
#include "CommandQueueManager.h"

namespace Uncertain
{
	Command::Command(CommandName name, ComQueueName exeQueue)
		:CName(name), ExeQueue(exeQueue), pCallback(nullptr), bPreventCallback(false)
	{
	}

	Command::Command(CommandName name, ComQueueName exeQueue, Command* _pCallback)
		: CName(name), ExeQueue(exeQueue), pCallback(_pCallback), bPreventCallback(false)
	{
	}

	Command::~Command()
	{

		if (!this->bPreventCallback && this->pCallback != nullptr)
		{
			if (this->ExeQueue == this->pCallback->ExeQueue)
			{
				this->pCallback->Execute();
			}
			else
			{
				CommandQueueManager::SendData(this->pCallback);
			}
		}
	}

	void Command::Send()
	{
		CommandQueueManager::SendData(this);
	}

	void Command::SetCallback(Command* _pCallback)
	{
		this->pCallback = _pCallback;
	}

	void Command::PreventCallback()
	{
		this->bPreventCallback = true;
	}

	ComPackage::ComPackage(ComQueueName exeQueue)
		: Command(CommandName::ComPackage, exeQueue), CommandCount(0)
	{
	}

	void ComPackage::Execute()
	{
		for (uint32_t i = 0; i < CommandCount; ++i)
		{
			Commands[i]->Execute();
		}
	}

	int ComPackage::AddCommand(Command* pCom)
	{
		if (CommandCount < MAX_COMMAND_COUNT)
		{
			Commands[CommandCount++] = pCom;
			return CommandCount;
		}
		else
		{
			return -1;
		}
	}

}
