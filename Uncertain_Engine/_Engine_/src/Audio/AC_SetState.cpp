#include "AC_SetState.h"
#include "AK/SoundEngine/Common/AkSoundEngine.h"

Uncertain::AC_SetState::AC_SetState(AkUniqueID groupID, AkUniqueID stateID)
	: Command(CommandName::Audio_SetState, ComQueueName::AUDIO), GroupID(groupID), StateID(stateID)
{
}

void Uncertain::AC_SetState::Execute()
{
	AK::SoundEngine::SetState(GroupID, StateID);
}
