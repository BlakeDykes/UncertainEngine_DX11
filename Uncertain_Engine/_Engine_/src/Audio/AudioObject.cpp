#include "AudioObject.h"
#include "AC_CB_DAO.h"
#include <AK/Wwise_IDs.h>

namespace Uncertain
{
#pragma warning( push )
#pragma warning( disable : 4826 )
#pragma warning( disable : 26495 )
	AudioObject::AudioObject()
		: ID(reinterpret_cast<AkGameObjectID>(this))
		, poDAO(nullptr)
		, poPluginInfo(nullptr)
	{
	}
#pragma warning( pop )

	AudioObject::~AudioObject()
	{
		if (poDAO)
		{
			poDAO->Destroy_GameAlloc();
			delete poDAO;
		}

		delete poPluginInfo;
	}

	void AudioObject::ProcessRequests(std::list<SpectralRequest*>::iterator beginPos, std::list<SpectralRequest*>::iterator endPos)
	{
		if (poDAO) this->poDAO->ProcessRequests(beginPos, endPos);
	}

	void AudioObject::SetPluginInfo(AudioPluginInfo* info)
	{
		assert(info);

		if (poPluginInfo)
		{
			*poPluginInfo = *info;
		}
		else
		{
			poPluginInfo = new AudioPluginInfo(*info);
		}
	}

	void AudioObject::SetRequestedPluginData(RequestedData reqData, RequestedChannel reqChannel)
	{
		assert(poDAO == nullptr);
		this->poDAO = new SpectralDAO();
		this->poDAO->Init_GameAlloc(reqData, reqChannel);
	}

	void* AudioObject::GetCallbackCookie(UTWWISE::CallbackCookies cookieType)
	{
		switch (cookieType)
		{
		case EngineNames::UTWWISE::CallbackCookies::None:			return 0;
		case EngineNames::UTWWISE::CallbackCookies::RegisterDAO:	return GetRegisterDAOCookie();
		case EngineNames::UTWWISE::CallbackCookies::UnregisterDAO:	return GetUnregisterDAOCookie();
		
		default:													return 0;
		}
	}

	void* AudioObject::GetRegisterDAOCookie() 
	{ 
		return (void*)new AC_CB_RegisterDAO(ID, this->poDAO, poPluginInfo);
	}
	void* AudioObject::GetUnregisterDAOCookie() { return (void*)new AC_CB_UnregisterDAO(ID, poPluginInfo); }
}
