#pragma once

#include "FileHeaders.h"

// Make the assumption of c-char strings, not UNICODE
namespace Uncertain
{
	class File
	{
	public:

		// Constants for the library
		static const uint32_t MAJOR_VERSION = 2;
		static const uint32_t MINOR_VERSION = 0;

		typedef void* Handle;

		enum class Mode : DWORD
		{
			READ = 0x80000000L,
			WRITE = 0x40000000L,
			READ_WRITE = (0x80000000L | 0x40000000L)
		};

		enum class Position : uint32_t
		{
			BEGIN = 0x7B000000,
			CURRENT,
			END
		};

		enum class Error : uint32_t
		{
			SUCCESS = 0x7C000000,
			OPEN_FAIL,
			CLOSE_FAIL,
			WRITE_FAIL,
			READ_FAIL,
			SEEK_FAIL,
			TELL_FAIL,
			FLUSH_FAIL,
			DIRECTORY_EXISTS,
			PATH_FAIL,
			INVALID_HANDLE,
			UNSUPORTED_FILE_TYPE,
			UNDEFINED
		};

	public:
		static File::Error Open(File::Handle& fh, const char* const fileName, File::Mode mode) noexcept;
		static File::Error Close(File::Handle& fh) noexcept;
		static File::Error Write(File::Handle fh, const void* const buffer, const DWORD inSize) noexcept;
		static File::Error Read(File::Handle fh, void* const _buffer, const DWORD _size) noexcept;
		static File::Error Seek(File::Handle fh, File::Position location, int offset) noexcept;
		static File::Error MkDir(const char* const dirName) noexcept;
		static File::Error Tell(File::Handle fh, DWORD& offset) noexcept;
		static File::Error Flush(File::Handle fh) noexcept;
		static File::Error GetSize(File::Handle fh, DWORD& sizeOut) noexcept;
		static bool IsHandleValid(File::Handle fh) noexcept;

		static File::Error GetRIFFHeaders(File::Handle& fh, RIFF::Header& headerOut);
		static File::Error ReadChunkData(File::Handle& fh, void* pBuffer, const RIFF::Header& header);
	};
}

// --- End of File ---
