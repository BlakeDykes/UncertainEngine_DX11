#pragma once

#include "Command.h"
#include "AudioPluginInfo.h"

class CustomData;

namespace Uncertain
{
	class AC_CB : public Command
	{
	public:
		AC_CB(CommandName cname, AudioPluginInfo* info, CustomData* data);
		virtual ~AC_CB();

		virtual void Execute() override;

		size_t GetDataSize();

	protected:
		AudioPluginInfo* pInfo;
		CustomData* poData;
	};
}