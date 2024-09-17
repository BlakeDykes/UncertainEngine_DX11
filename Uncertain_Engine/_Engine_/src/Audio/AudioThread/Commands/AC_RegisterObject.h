#pragma once

#include "Command.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

namespace Uncertain
{
	class AC_RegisterObject : public Command
	{
	public:
		AC_RegisterObject(const AkGameObjectID& id, const char* debugName);

		// Inherited via Command
		void Execute() override;

	private:
		AkGameObjectID ID;
		const char* DebugName;
	};
}
