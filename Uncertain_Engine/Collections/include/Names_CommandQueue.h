#pragma once

namespace Uncertain
{
	namespace CommandQueueNames
	{
		typedef int COMMAND_QUEUE_ID;

		enum class CommandName
		{
			// Anim
			Anim_Set,
			Anim_SetNoBlend,
			Anim_SetIdleAsActive,

			// Text
			SetMessage,

			// Timed Events
			TimedEvent_Anim,

			CB_Done,

			UNINITIALIZED
		};

		enum class QueueName
		{
			GAME,
			ANIM,
			AUDIO,
			FILE,
			AUX,
			UCallback,

			UNINITIALIZED
		};
	}
}

using namespace Uncertain::CommandQueueNames;

