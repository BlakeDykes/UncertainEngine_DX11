#include "AC_PostEvent.h"
#include "_Helpers.h"

Uncertain::AC_PostEvent::AC_PostEvent(AkUniqueID in_eventID, AkGameObjectID in_gameObjectID, AkUInt32 in_uFlags,
	AkCallbackFunc in_pfnCallback, void* in_pCookie, AkUInt32 in_cExternals,
	AkExternalSourceInfo* in_pExternalSources, AkPlayingID in_PlayingID)
	: Command(CommandName::Audio_PostEvent, ComQueueName::AUDIO), EventID(in_eventID), GameObjectID(in_gameObjectID),
	Flags(in_uFlags), Callback(in_pfnCallback), pCookie(in_pCookie), Externals(in_cExternals),
	pExternalSources(in_pExternalSources), PlayingID(in_PlayingID)
{
}

void Uncertain::AC_PostEvent::Execute()
{
	AK::SoundEngine::PostEvent(EventID, GameObjectID, Flags, Callback, pCookie, Externals, pExternalSources, PlayingID);
	
	PRINTCOMMAND("Post Event", EventID, GameObjectID)
}

