#include "AC_RegisterObject.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

Uncertain::AC_RegisterObject::AC_RegisterObject(const AkGameObjectID& id, const char* debugName)
	: Command(CommandName::Audio_Register, ComQueueName::AUDIO), ID(id), DebugName(debugName)
{
}

void Uncertain::AC_RegisterObject::Execute()
{
	AK::SoundEngine::RegisterGameObj(ID, DebugName);
}
