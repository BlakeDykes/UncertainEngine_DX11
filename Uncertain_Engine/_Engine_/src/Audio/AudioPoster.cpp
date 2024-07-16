#include "AudioPoster.h"
#include "AudioObject.h"
#include "_AudioCommandWrapper.h"
#include "SA_FFT.h"

namespace Uncertain
{
	void AudioPoster::Register(AudioObject& object, const char* debugName) const
	{
		AC_RegisterObject* com = new AC_RegisterObject(object.GetID(), debugName);
		com->Send();
	}

	void AudioPoster::Unregister(AkGameObjectID objectID) const
	{
		AC_UnregisterObject* com = new AC_UnregisterObject(objectID);
		com->Send();
	}

	void AudioPoster::PostEvent(AkUniqueID in_eventID, AkGameObjectID in_gameObjectID, AkUInt32 in_uFlags,
		AkCallbackFunc in_pfnCallback, void* in_pCookie, AkUInt32 in_cExternals, 
		AkExternalSourceInfo* in_pExternalSources, AkPlayingID in_PlayingID) const
	{
		AC_PostEvent* com = new AC_PostEvent(in_eventID, in_gameObjectID, in_uFlags, in_pfnCallback,
			in_pCookie, in_cExternals, in_pExternalSources, in_PlayingID);
		com->Send();
	}

	void AudioPoster::SendPluginGameData(AkGameObjectID goid, AudioPluginInfo* pInfo, const void* in_pData, AkUInt32 in_uSizeInBytes) const
	{
		AC_SendPluginGameData* com = new AC_SendPluginGameData(goid, pInfo, in_pData, in_uSizeInBytes);
		com->Send();
	}

	void AudioPoster::SetState(AkUniqueID groupID, AkUniqueID stateID) const
	{
		AC_SetState* com = new AC_SetState(groupID, stateID);
		com->Send();
	}
	void AudioPoster::SetDefaultListener(AkGameObjectID listenerID) const
	{
		AC_SetDefaultListener* com = new AC_SetDefaultListener(listenerID);
		com->Send();
	}

	void AudioPoster::SetListener(AkGameObjectID emitterID, AkGameObjectID listenerID) const
	{
		AC_SetListener* com = new AC_SetListener(emitterID, listenerID);
		com->Send();
	}
}

