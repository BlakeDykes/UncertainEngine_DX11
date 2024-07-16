#include "FileThread.h"
#include "File.h"
#include "Buffer.h"
#include "FileCoordinator.h"
#include "Command.h"

namespace Uncertain
{

	void FileThread::Create(PromiseFlag* readyFlag)
	{
		Instance()->Launch(Priority::NORMAL, readyFlag);
	}

	FileThread::FileThread()
		: QueueThreadBase(ComQueueName::FILE),
		poBuffer(new Buffer(FileThread::BUFFER_SIZE))
	{
	}

	FileThread::~FileThread()
	{
		delete poBuffer;
	}

	void FileThread::ThreadMain(PromiseFlag* readyFlag)
	{
		if (readyFlag) readyFlag->Flag();

		while (!ShouldShutdown())
		{
			Command* pCom = nullptr;
			while (this->InQueue->PopFront(pCom))
			{
				pCom->Execute();
			}

			//std::this_thread::sleep_for(200ms);
		}
	}

	void FileThread::LoadFile(const char* const pFileName, std::promise<Buffer*&> coordTransfer)
	{
		File::Handle fh;
		File::Error err;

		FileThread* inst = Instance();
		
		assert(pFileName);

		//Debug::out("%s load start <-- \n", pFileName);

		err = File::Open(fh, pFileName, File::Mode::READ);
		assert(err == File::Error::SUCCESS);

		err = File::Seek(fh, File::Position::END, 0);
		assert(err == File::Error::SUCCESS);

		DWORD size;
		err = File::Tell(fh, size);
		assert(err == File::Error::SUCCESS);

		err = File::Seek(fh, File::Position::BEGIN, 0);
		assert(err == File::Error::SUCCESS);

		err = File::Read(fh, inst->poBuffer->GetRawBuffer(), size);
		assert(err == File::Error::SUCCESS);

		err = File::Close(fh);
		assert(err == File::Error::SUCCESS);

		inst->poBuffer->SetFilled(size);
		
		coordTransfer.set_value(std::ref(inst->poBuffer));

		//Debug::out("%s load end --> %d bytes \n", pFileName, poBuffer->GetCurrSize());
	}

	void FileThread::LoadFile(File::Handle openFH, size_t size, size_t offset, Buffer* bufferOut)
	{
		File::Error err;

		FileThread* inst = Instance();

		//Debug::out("%s load start <-- \n", pFileName);

		assert(File::IsHandleValid(openFH));

		err = File::Seek(openFH, File::Position::BEGIN, offset);
		assert(err == File::Error::SUCCESS);

		err = File::Read(openFH, bufferOut->GetRawBuffer(), size);
		assert(err == File::Error::SUCCESS);

		bufferOut->SetFilled(size);
	}


	FileThread* FileThread::Instance()
	{
		static FileThread instance;
		return &instance;
	}
}