#include "AC_UnregisterObject.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

Uncertain::AC_UnregisterObject::AC_UnregisterObject(const AkGameObjectID& id)
	: Command(CommandName::Audio_Unregister, ComQueueName::AUDIO), ID(id)
{
}

void Uncertain::AC_UnregisterObject::Execute()
{
	AK::SoundEngine::UnregisterGameObj(ID);
}
