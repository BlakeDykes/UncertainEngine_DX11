#include "AudioPluginInfo.h"

namespace Uncertain
{

	AudioPluginInfo::AudioPluginInfo(
		AkUInt32 			pluginID
		, AkUniqueID 		busID
		, AkPluginType 		eType
		, AkUInt32 			companyID
	)
		: PluginID(pluginID)
		, BusID(busID)
		, EType(eType)
		, CompanyID(companyID)
	{
	}

	AudioPluginInfo::AudioPluginInfo(AudioPluginInfo& b)
	{
		this->PluginID = b.PluginID;
		this->BusID = b.BusID;
		this->EType = b.EType;
		this->CompanyID = b.CompanyID;

	}

	AudioPluginInfo& AudioPluginInfo::operator=(AudioPluginInfo& b)
	{
		if (this != &b)
		{
			this->PluginID = b.PluginID;
			this->BusID = b.BusID;
			this->EType = b.EType;
			this->CompanyID = b.CompanyID;
		}

		return *this;
	}

	void AudioPluginInfo::Set(
		AkUInt32 			pluginID
		, AkUniqueID 		busID
		, AkPluginType 		eType
		, AkUInt32 			companyID
	)
	{
		PluginID = pluginID;
		BusID = busID;
		EType = eType;
		CompanyID = companyID;
	}

	inline bool AudioPluginInfo::operator==(const AudioPluginInfo& b)
	{
		return PluginID		== b.PluginID
			&& BusID		== b.BusID
			;
	}

	inline bool AudioPluginInfo::operator!=(const AudioPluginInfo& b)
	{
		return PluginID		!= b.PluginID
			|| BusID		!= b.BusID
			;
	}
}