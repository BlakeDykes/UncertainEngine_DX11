#pragma once

#include "AC_CB.h"
#include "CD_DAO.h"

namespace Uncertain
{
	class AC_CB_RegisterDAO : public AC_CB
	{
	public:
		AC_CB_RegisterDAO(AkGameObjectID goid, SpectralDAO* gameDAO, AudioPluginInfo* info)
			: AC_CB(CommandName::Audio_CB_RegisterSpectralDAO, info, new CD_RegisterDAO(gameDAO, goid))
		{}

	};

	class AC_CB_UnregisterDAO : public AC_CB
	{
	public:
		AC_CB_UnregisterDAO(AkGameObjectID goid, AudioPluginInfo* info)
			: AC_CB(CommandName::Audio_CB_RegisterSpectralDAO, info, new CD_UnregisterDAO(goid))
		{}
	};
}


