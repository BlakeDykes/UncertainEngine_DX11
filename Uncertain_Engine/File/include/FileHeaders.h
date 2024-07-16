#pragma once

namespace Uncertain
{
	namespace RIFF
	{
		namespace FileTypes
		{
		#ifdef _XBOX
			static const int fourccRIFF = 'RIFF';
			static const int fourccDATA = 'data';
			static const int fourccFMT = 'fmt';
			static const int fourccWAVE = 'WAVE';
			static const int fourccXWMA = 'XWMA';
			static const int fourccDPDS = 'dpds';
		#else
			static const int fourccRIFF = 'FFIR';
			static const int fourccDATA = 'atad';
			static const int fourccFMT = ' tmf';
			static const int fourccWAVE = 'EVAW';
			static const int fourccXWMA = 'AMWX';
			static const int fourccDPDS = 'sdpd';
		#endif
		};

		struct Header
		{
			DWORD FileType;
			LONG TotalFileSize;
			WAVEFORMATEX* poWaveHeader;
			DWORD DataOffset;
			DWORD DataSize;

			Header();
			~Header();
			void Wash();
		};
	}
}