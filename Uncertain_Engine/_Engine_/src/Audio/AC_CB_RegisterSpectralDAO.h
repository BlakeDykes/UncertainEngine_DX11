#pragma once

#include "Command.h"
#include "SpectralDAO.h"
#include "AudioPluginInfo.h"

namespace Uncertain
{
	class AC_CB_RegisterSpectralDAO : public Command
	{
	public:
		AC_CB_RegisterSpectralDAO(SpectralDAO* gameDAO, AudioPluginInfo* info);


		// Inherited via Command
		void Execute() override;

	private:
		AudioPluginInfo* pInfo;
		SpectralDAO* pGameDAO;
		
	};
}


