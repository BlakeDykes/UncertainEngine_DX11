#pragma once

#include "Command.h"
#include "Buffer.h"
#include "ThreadFlags.h"

namespace Uncertain
{
	class FC_LoadFiles : public Command
	{
		FC_LoadFiles(const char* const fileName, std::promise<Buffer*&> coordTransfer, FileToCoordCompleteFlag& transferComplete);

		// Inherited via Command
		void Execute() override;

	private:
		const char* const pFileName;
		std::promise<Buffer*&> prCoordTransfer;
		FileToCoordCompleteFlag& FileToCoordComplete;
	};
}
