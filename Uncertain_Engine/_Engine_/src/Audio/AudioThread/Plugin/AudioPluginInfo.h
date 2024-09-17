#pragma once

#include < AK/SoundEngine/Common/AkCommonDefs.h >

namespace Uncertain
{
	struct AudioPluginInfo
	{
		AkUInt32 		PluginID;
		AkUniqueID 		BusID;
		AkPluginType 	EType;
		AkUInt32 		CompanyID;

		AudioPluginInfo() = default;
		AudioPluginInfo(AudioPluginInfo& b);
		AudioPluginInfo& operator=(AudioPluginInfo& b);
		~AudioPluginInfo() = default;

		AudioPluginInfo(
			AkUInt32 			pluginID
			, AkUniqueID 		busID
			, AkPluginType 		eType
			, AkUInt32 			companyID
		);

		inline void Set(
			AkUInt32 			pluginID
			, AkUniqueID 		busID
			, AkPluginType 		eType
			, AkUInt32 			companyID
		);
		
		inline bool operator==(const AudioPluginInfo& b);
		inline bool operator!=(const AudioPluginInfo& b);
	};
}