#include "AC_SetDefaultListener.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

Uncertain::AC_SetDefaultListener::AC_SetDefaultListener(AkGameObjectID listener)
	: Command(CommandName::Audio_SetDefaultListener, ComQueueName::AUDIO), ListenerID(listener)
{
}

void Uncertain::AC_SetDefaultListener::Execute()
{
	AK::SoundEngine::SetDefaultListeners(&ListenerID, 1);
}
