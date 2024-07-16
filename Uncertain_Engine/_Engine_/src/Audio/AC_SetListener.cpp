#include "AC_SetListener.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

Uncertain::AC_SetListener::AC_SetListener(AkGameObjectID emitter, AkGameObjectID listener)
	: Command(CommandName::Audio_SetListener, ComQueueName::AUDIO), EmitterID(emitter), ListenerID(listener)
{
}

void Uncertain::AC_SetListener::Execute()
{
	AK::SoundEngine::SetListeners(EmitterID, &ListenerID, 1);
}
