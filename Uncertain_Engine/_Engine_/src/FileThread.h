#pragma once

#include "ThreadBase.h"
#include "Buffer.h"
#include "File.h"

namespace Uncertain
{
	class FileThread : public QueueThreadBase
	{
	public:
		static const unsigned int BUFFER_SIZE = 512 * 1024;

		static void Create(PromiseFlag* readyFlag = nullptr);
		static void LoadFile(const char* const pFileName, std::promise<Buffer*&> coordTransfer);
		static void LoadFile(File::Handle openFH, size_t size, size_t offset, Buffer* bufferOut);

		FileThread();
		virtual ~FileThread();

	protected:
		virtual void ThreadMain(PromiseFlag* readyFlag) override;

	private:
		static FileThread* Instance();

		Buffer* poBuffer;
	};
}