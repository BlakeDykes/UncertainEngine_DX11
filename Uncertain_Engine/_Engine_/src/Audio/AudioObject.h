#pragma once

#include "AudioPoster.h"
#include "AudioObjectManager.h"
#include "AudioPluginInfo.h"
#include "SpectralRequest.h"

namespace Uncertain
{
	class AudioObject
	{
	public:
		AudioObject();
		AudioObject(const AudioObject&) = delete;
		AudioObject& operator=(const AudioObject&) = delete;
		virtual ~AudioObject();

		void ProcessRequests(
			std::list<SpectralRequest*>::iterator beginPos
			, std::list<SpectralRequest*>::iterator endPos
		);

		void SetPluginInfo(AudioPluginInfo* info);

		void SetRequestedPluginData(RequestedData reqData, RequestedChannel reqChannel);

		AkGameObjectID GetID() const { return ID; }
		AudioPluginInfo* GetPluginInfo() { return poPluginInfo; }
		void* GetCallbackCookie(UTWWISE::CallbackCookies cookieType);

	private:
		AkGameObjectID ID;
		AudioPluginInfo* poPluginInfo;
		SpectralDAO* poDAO;

		void* GetRegisterDAOCookie();
		void* GetUnregisterDAOCookie();

	};
}