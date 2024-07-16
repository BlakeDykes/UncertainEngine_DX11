#include "AC_SendPluginGameData.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>
#include "_Helpers.h"

Uncertain::AC_SendPluginGameData::AC_SendPluginGameData(
	  AkGameObjectID   goid
	, AudioPluginInfo* info
	, const void*	   data
	, AkUInt32 		   sizeInBytes
)
	: Command(CommandName::Audio_SendPluginGameData, ComQueueName::AUDIO)
	, GOID(goid)
	, pInfo(info)
	, pData(data)
	, SizeInBytes(sizeInBytes)
{
}

void Uncertain::AC_SendPluginGameData::Execute()
{
	AK::SoundEngine::SendPluginCustomGameData(
		pInfo->BusID
		, GOID
		, pInfo->EType
		, pInfo->CompanyID
		, pInfo->PluginID
		, pData
		, SizeInBytes
	);
}
