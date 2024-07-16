#pragma once

namespace Uncertain
{
	class Command
	{
	public:
		Command() = delete;
		Command(CommandName name, ComQueueName exeQueue);
		Command(CommandName name, ComQueueName exeQueue, Command* _pCallback);
		Command(const Command&) = default;
		Command& operator =(const Command&) = default;
		virtual ~Command();

		virtual void Send();

		CommandName GetName() { return this->CName; }
		ComQueueName GetExeQueue() { return this->ExeQueue; }
		void SetCallback(Command* _pCallback);

		virtual void Execute() = 0;

	protected:
		virtual void PreventCallback();

		CommandName CName;
		ComQueueName ExeQueue;
		Command* pCallback;
		BOOL bPreventCallback;
	};

	class ComPackage : public Command
	{
		static const int MAX_COMMAND_COUNT = 5;

		ComPackage(ComQueueName exeQueue);

		// Inherited via Command
		void Execute() override;

		int AddCommand(Command* pCom);
		uint32_t GetCommandCount() const { return this->CommandCount; }

	protected:
		uint32_t CommandCount;
		Command* Commands[MAX_COMMAND_COUNT];
	};
}
