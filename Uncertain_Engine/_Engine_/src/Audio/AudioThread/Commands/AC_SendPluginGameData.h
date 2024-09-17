#pragma once

#include "Command.h"
#include "Audio/AudioThread/Plugin/AudioPluginInfo.h"

namespace Uncertain
{
	class AC_SendPluginGameData : public Command
	{
	public:
		AC_SendPluginGameData(
			AkGameObjectID   goid,
			AudioPluginInfo* info,
			const void*		 data,
			AkUInt32 		 sizeInBytes
		);

		// Inherited via Command
		void Execute() override;

	private:
		AkGameObjectID	 GOID;
		AudioPluginInfo* pInfo;
		const void*		 pData;
		AkUInt32		 SizeInBytes;

	};
}
