#pragma once

#include "Command.h"
#include <AK/SoundEngine/Common/AkCommonDefs.h>

namespace Uncertain
{
	class AC_SetDefaultListener : public Command
	{
	public:
		AC_SetDefaultListener(AkGameObjectID listener);

		// Inherited via Command
		void Execute() override;

	private:
		AkGameObjectID			ListenerID;

	};
}
