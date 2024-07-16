#pragma once

#include "DLink.h"
#include "AudioObjectManager.h"
#include "AudioPoster.h"
#include <unordered_map>
#include <list>

class SpectralRequest;

namespace Uncertain
{
	class AudioObject;

	static AkGameObjectID DEFAULT_EMITTER = 0;

	class AudioNode : public DLink
	{
	public:
		AudioNode();
		AudioNode(const AudioNode&) = delete;
		AudioNode& operator=(const AudioNode&) = delete;
		virtual ~AudioNode();

		void Update();

		void Set(const char* debugID, AudioPoster* globalPoster);

		AkGameObjectID AddListener(const char* debugName, AkUniqueID emitterID = DEFAULT_EMITTER);
		void AddSpectralRequest(SpectralRequest& req);
		void RemoveSpectralRequest(SpectralRequest& req);

		void PostEvent(
			  AkUniqueID				in_eventID
			, AkUInt32					in_uFlags
			, AkCallbackFunc			in_pfnCallback
			, UTWWISE::CallbackCookies	cookieType
			, AkUInt32					in_cExternals
			, AkExternalSourceInfo*		in_pExternalSources
			, AkPlayingID 				in_PlayingID
		);

		void SendPluginGameData(const void* in_pData, AkUInt32 in_uSizeInBytes);

		void SetAsDefaultListener(AkGameObjectID gameObjID);
		void SetPluginInfo(AudioPluginInfo* info);
		void SetRequestedPluginData(RequestedData reqData, RequestedChannel reqChannel);

		// Inherited via DLink
		//void Wash() override { delete poModel; poModel = nullptr; }
		bool Compare(NodeBase& pNode) override;
		void Wash() override;

	private:
		AudioPoster* pPoster;
		AudioObject* poObject;
		std::unordered_map<AkGameObjectID, AudioObject*> Listeners;
		std::list<SpectralRequest*> SpecRequests;

	#ifdef _DEBUG
		std::string DebugID;
	#endif // _DEBUG
	};
}

