#pragma once

#include "ManagerBase.h"
#include <AK/SoundEngine/Common/AkCallback.h>
#include "SpectralDAO.h"

namespace Uncertain
{
	class AudioObject;
	class AudioNode;
	class AudioPoster;
	struct AudioPluginInfo;

	class AudioObjectManager : public ManagerBase
	{
	public:
		virtual ~AudioObjectManager();

		static AudioObjectManager* Create();
		
		AudioNode* CreateAudioNode(const char* debugName = "Audio Object");
		void RemoveAudioNode(AudioNode& node);

		const AudioPoster*& GetPoster() { return (const AudioPoster*&)Instance()->poPoster; }

		void AddGlobalSpectralRequest(SpectralRequest& req);

		void PostGlobalEvent(AkUniqueID in_eventID,
			AkUInt32 in_uFlags,
			AkCallbackFunc in_pfnCallback,
			UTWWISE::CallbackCookies cookieType,
			AkUInt32 in_cExternals,
			AkExternalSourceInfo* in_pExternalSources,
			AkPlayingID 	in_PlayingID 
		);

		void Update();

		void SetGlobalPluginInfo(AudioPluginInfo* info);
		void SetGlobalRequestedPluginData(RequestedData reqData, RequestedChannel reqChannels);
		void SetGlobalState(AkUniqueID stateGroupID, AkUniqueID state);

	private:
		AudioNode* poGlobalAudio;
		AudioPoster* poPoster;

		AudioObjectManager();
		AudioObjectManager(const AudioObjectManager&) = delete;
		AudioObjectManager& operator =(const AudioObjectManager&) = delete;

		static AudioObjectManager* instance;
		static AudioObjectManager* Instance();

		// Inherited via ManagerBase
		NodeBase* CreateNode() override;
	};
}
