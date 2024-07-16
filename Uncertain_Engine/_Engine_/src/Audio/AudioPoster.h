#pragma once

#include "AudioObjectManager.h"
#include "AudioPluginInfo.h"

namespace Uncertain
{
	class AudioObject;

	class AudioPoster
	{
	public:

		virtual void Register(AudioObject& object, const char* debugName) const;

		virtual void Unregister(AkGameObjectID objectID) const;

		virtual void PostEvent(	
			AkUniqueID in_eventID,
			AkGameObjectID in_gameObjectID,
			AkUInt32 in_uFlags,
			AkCallbackFunc in_pfnCallback,
			void* in_pCookie,
			AkUInt32 in_cExternals,
			AkExternalSourceInfo* in_pExternalSources,
			AkPlayingID 	in_PlayingID 
		) const;

		void SendPluginGameData(AkGameObjectID goid, AudioPluginInfo* pInfo, const void* in_pData, AkUInt32 in_uSizeInBytes) const;

		virtual void SetState(AkUniqueID groupID, AkUniqueID stateID) const;

		virtual void SetDefaultListener(AkGameObjectID listenerID) const;
		virtual void SetListener(AkGameObjectID emitterID, AkGameObjectID listenerID) const;
	};
}
