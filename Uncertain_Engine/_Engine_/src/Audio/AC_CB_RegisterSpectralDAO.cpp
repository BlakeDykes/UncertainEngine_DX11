#include "AC_CB_RegisterSpectralDAO.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>


Uncertain::AC_CB_RegisterSpectralDAO::AC_CB_RegisterSpectralDAO(SpectralDAO* gameDAO, AudioPluginInfo* info)
	: Command(CommandName::Audio_CB_RegisterSpectralDAO, ComQueueName::UNINITIALIZED)
	, pInfo(info)
	, pGameDAO(gameDAO)
{
}

void Uncertain::AC_CB_RegisterSpectralDAO::Execute()
{
	AK::SoundEngine::SendPluginCustomGameData(
		pInfo->BusID
		, pInfo->BusObjectID
		, pInfo->EType
		, pInfo->CompanyID
		, pInfo->PluginID
		, (void*)pGameDAO
		, sizeof(SpectralDAO)
	);
}
