#pragma once

#include "Command.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

namespace Uncertain
{
	class AC_UnregisterObject : public Command
	{
	public:
		AC_UnregisterObject(const AkGameObjectID& id);

		// Inherited via Command
		void Execute() override;

	private:
		const AkGameObjectID& ID;
	};
}
