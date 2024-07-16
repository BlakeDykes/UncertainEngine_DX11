#include "FC_LoadFiles.h"
#include "FileThread.h"

namespace Uncertain
{
	FC_LoadFiles::FC_LoadFiles(const char* const fileName, std::promise<Buffer*&> coordTransfer, FileToCoordCompleteFlag& transferComplete)
		: Command(CommandName::File_Load, ComQueueName::FILE), pFileName(fileName), 
		prCoordTransfer(std::move(coordTransfer)), FileToCoordComplete(transferComplete)
	{
	}

	void FC_LoadFiles::Execute()
	{
		FileThread::LoadFile(pFileName, std::move(prCoordTransfer));

		std::unique_lock<std::mutex> m(FileToCoordComplete.mtx);
		FileToCoordComplete.cv.wait(m);
	}
}

