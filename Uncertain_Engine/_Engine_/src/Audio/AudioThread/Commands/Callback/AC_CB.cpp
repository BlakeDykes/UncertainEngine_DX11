#include "AC_CB.h"
#include "CustomData.h"

#include <AK/SoundEngine/Common/AkSoundEngine.h>

namespace Uncertain
{
	AC_CB::AC_CB(CommandName cname, AudioPluginInfo* info, CustomData* data)
		: Command(cname, ComQueueName::UNINITIALIZED), pInfo(info), poData(data)
	{
	}

	AC_CB::~AC_CB()
	{
		delete this->poData;
	}

	void AC_CB::Execute()
	{
		AKRESULT res = AK::SoundEngine::SendPluginCustomGameData(
			pInfo->BusID
			, AK_INVALID_GAME_OBJECT
			, pInfo->EType
			, pInfo->CompanyID
			, pInfo->PluginID
			, (void*)poData
			, poData->GetThisSize()
		);

		assert(res == AK_Success);
	}

	size_t AC_CB::GetDataSize()
	{
		return poData->GetThisSize();
	}
}
