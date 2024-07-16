#include "FileHeaders.h"

namespace Uncertain
{
	RIFF::Header::Header()
		: FileType(0), TotalFileSize(0), poWaveHeader(nullptr),
		DataOffset(0)
	{
	}
	
	RIFF::Header::~Header()
	{
		delete poWaveHeader;
	}

	void RIFF::Header::Wash()
	{
		FileType = 0;
		TotalFileSize = 0;
		memset(poWaveHeader, 0x0, sizeof(WAVEFORMATEX));
		DataOffset = 0;
	}
}