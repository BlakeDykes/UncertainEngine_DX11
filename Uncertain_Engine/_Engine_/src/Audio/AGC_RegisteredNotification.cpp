#include "AGC_RegisteredNotification.h"
#include "AudioObjectManager.h"

Uncertain::AGC_RegisteredNotification::AGC_RegisteredNotification(AudioObject& obj)
	: Command(CommandName::Audio_Register, ComQueueName::AUDIO_GAME), Object(obj)
{
}

void Uncertain::AGC_RegisteredNotification::Execute()
{
	Object.SetPoster(AudioObjectManager::GetPoster());
}
