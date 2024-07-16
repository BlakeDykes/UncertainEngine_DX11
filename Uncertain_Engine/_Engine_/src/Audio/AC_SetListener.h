#pragma once

#include "Command.h"
#include <AK/SoundEngine/Common/AkCommonDefs.h>

namespace Uncertain
{
	class AC_SetListener : public Command
	{
	public:
		AC_SetListener(AkGameObjectID emitter, AkGameObjectID listener);

		// Inherited via Command
		void Execute() override;

	private:
		AkGameObjectID			EmitterID;
		AkGameObjectID			ListenerID;

	};
}
