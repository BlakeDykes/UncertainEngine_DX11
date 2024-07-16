#pragma once

#include "Command.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

namespace Uncertain
{
	class AC_PostEvent : public Command
	{
	public:
		AC_PostEvent(AkUniqueID in_eventID,
			AkGameObjectID in_gameObjectID,
			AkUInt32 in_uFlags,
			AkCallbackFunc in_pfnCallback,
			void* in_pCookie,
			AkUInt32 in_cExternals,
			AkExternalSourceInfo* in_pExternalSources,
			AkPlayingID in_PlayingID
		);

		// Inherited via Command
		void Execute() override;

	private:
		AkUniqueID				EventID;
		AkGameObjectID			GameObjectID;
		AkUInt32				Flags;
		AkCallbackFunc			Callback;
		void*					pCookie;
		AkUInt32				Externals;
		AkExternalSourceInfo*	pExternalSources;
		AkPlayingID				PlayingID;
	};
}
