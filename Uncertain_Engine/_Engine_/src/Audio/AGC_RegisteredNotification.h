#pragma once

#include "Command.h"
#include "AudioObject.h"

namespace Uncertain
{
	class AGC_RegisteredNotification : public Command
	{
	public:
		AGC_RegisteredNotification(AudioObject& obj);

		// Inherited via Command
		void Execute() override;

	private:
		AudioObject& Object;
	};
}