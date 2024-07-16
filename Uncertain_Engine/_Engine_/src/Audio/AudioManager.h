#pragma once

#include "ThreadBase.h"
#include <AK/SoundEngine/Common/AkCallback.h>

#ifndef DBOUT
#define DBOUT( s )				\
{								\
	std::ostringstream os_;		\
	os_ << s;					\
	OutputDebugString(os_.str().c_str()); \
}		
#endif

//#define DEBUG_FRAMETIME

class CAkDefaultIOHookDeferred;
class SpectralRequest;

namespace Uncertain
{
	class AudioObjectManager;
	class AudioPoster;
	class AudioNode;
	struct AudioPluginInfo;

	class AudioManager : public QueueThreadBase
	{
	public:
		const static size_t SPECTRAL_FRAME_SIZE = 512;
		const static size_t SPECTRAL_PULL_SIZE = SPECTRAL_FRAME_SIZE * 4;

		AudioManager& operator = (const AudioManager&) = delete;
		AudioManager(const AudioManager&) = delete;

		static void Create(PromiseFlag* readyFlag = nullptr);
		virtual void Shutdown() override;

		static void CB_MusicSyncEntry(
			AkCallbackType in_eType, 
			AkCallbackInfo* ini_pCallbackInfo
		);

		static AudioNode* CreateAudioNode(const char* debugName = "Audio Object");
		static void RemoveAudioNode(AudioNode& node);

		static const AudioPoster*& GetPoster();

		static void PostGlobalEvent(
			AkUniqueID in_eventID
			, AkUInt32					in_uFlags			 = 0
			, AkCallbackFunc			in_pfnCallback		 = NULL
			, UTWWISE::CallbackCookies	in_pCookie			 = UTWWISE::CallbackCookies::None
			, AkUInt32					in_cExternals		 = 0
			, AkExternalSourceInfo*		in_pExternalSources  = NULL
			, AkPlayingID 				in_PlayingID		 = AK_INVALID_PLAYING_ID
		);

		static void SetGlobalState(AkUniqueID stateGroupID, AkUniqueID state);

		static void AddGlobalSpectralRequest(SpectralRequest* req);

	protected:
		// Inherited via ThreadBase
		void ThreadMain(PromiseFlag* readyFlag) override;

	private:
		CAkDefaultIOHookDeferred* poIO;
		AudioObjectManager* poObjectManager;
		AudioPluginInfo* poPluginInfo_SA;

		AudioManager();
		virtual ~AudioManager();

		void RenderAudio();

		bool InitWwise();
		bool InitAKMemManager();
		bool InitAKStreamManager();
		bool InitAKSoundEngine();
		bool InitAKMusicEngine();
		bool InitAKSpatialAudio();
		bool InitAKComs();

		bool LoadGlobalBanks();
		bool LoadPluginInfo();
		
		void TerminateEngine();

		static AudioManager* Instance();
	};
}
