#pragma once

#include "Command.h"
#include "AudioObjectManager.h"

namespace Uncertain
{
	class AC_SetState : public Command
	{
	public:
		AC_SetState(AkUniqueID groupID, AkUniqueID stateID);


		void Execute() override;
	private:
		AkUniqueID GroupID;
		AkUniqueID StateID;
	};
}