#include "AudioManager.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>					// Sound engine
#include <AK/SoundEngine/Common/AkSoundEngine.h>					// Sound engine
#include <AK/SoundEngine/Common/AkMemoryMgr.h>						// Memory Manager interface
#include <AK/SoundEngine/Common/AkModule.h>							// Default memory manager
#include <AK/SoundEngine/Common/IAkStreamMgr.h>						// Streaming Manager
#include <AK/Tools/Common/AkPlatformFuncs.h>						// Thread defines
#include <AK/MusicEngine/Common/AkMusicEngine.h>					// Music Engine
#include <AK/SpatialAudio/Common/AkSpatialAudio.h>					// Spatial audio module
#include <Wwise/AKSamples/SoundEngine/Win32/AkDefaultIOHookDeferred.h>
#include <AK/Wwise_IDs.h>

#include "SpectralAnalysisFXFactory.h"
#include "SpectralAnalysisConfig.h"
#include "SpectralRequest.h"

#include "pugixml.hpp"

#ifndef AK_OPTIMIZED
#include <AK/Comm/AkCommunication.h>
#endif // !AK_OPTIMIZED

#include "_Timer.h"
#include "AudioObjectManager.h"
#include "AudioNode.h"
#include "AudioPoster.h"
#include "_AudioThreadCommandWrapper.h"

namespace Uncertain
{
	AudioManager::AudioManager()
		: QueueThreadBase(ComQueueName::AUDIO)
		, poIO(nullptr)
		, poObjectManager(nullptr)
		, poPluginInfo_SA(nullptr)
	{
	}

	AudioManager::~AudioManager()
	{
		delete poObjectManager;
		delete poPluginInfo_SA;
	}

	void AudioManager::Create(PromiseFlag* started)
	{
		Instance()->Launch(Priority::NORMAL, started);
	}

	void AudioManager::Shutdown()
	{
		TerminateEngine();
		ThreadBase::Shutdown();
	}

	void AudioManager::ThreadMain(PromiseFlag* readyFlag)
	{
		InitWwise();

		poObjectManager = AudioObjectManager::Create();

		LoadGlobalBanks();
		LoadPluginInfo();

	#ifdef DEBUG_FRAMETIME
		_Timer* t = new _Timer();
		_Time frameTime;
	#endif

		if(readyFlag) readyFlag->Flag();

		while (!ShouldShutdown()) 
		{

		#ifdef DEBUG_FRAMETIME
			t->Tic();
		#endif

			Command* pCom = nullptr;
			while (this->InQueue->PopFront(pCom))
			{
				DBOUT("Executing Command\n")
				pCom->Execute();
			}

			poObjectManager->Update();

			RenderAudio();

		#ifdef DEBUG_FRAMETIME
			frameTime = t->Toc();
			//DBOUT("Frame Time = " << _Time::Quotient(frameTime, _Time(_Time::Duration::ONE_MILLISECOND)) << "ms\n");
		#endif
		}
	}

	void AudioManager::CB_MusicSyncEntry(AkCallbackType in_eType, AkCallbackInfo* ini_pCallbackInfo)
	{
		AZUL_UNUSED_VAR(in_eType);

		AkMusicSyncCallbackInfo* info = static_cast<AkMusicSyncCallbackInfo*>(ini_pCallbackInfo);

		if (info->pCookie)
		{
			Command* pCom = static_cast<Command*>(info->pCookie);
			CommandName name = pCom->GetName();

			switch (name)
			{
			case EngineNames::CommandName::Audio_CB_RegisterSpectralDAO:
			{
				AC_CB_RegisterDAO* cbCom = static_cast<AC_CB_RegisterDAO*>(pCom);

				cbCom->Execute();
				break;
			}
			
			case EngineNames::CommandName::Audio_CB_UnregisterSpectralDAO:
			{
				AC_CB_UnregisterDAO* cbCom = static_cast<AC_CB_UnregisterDAO*>(pCom);

				cbCom->Execute();
				break;
			}

			case EngineNames::CommandName::Light_Debug:
			case EngineNames::CommandName::Light_SetDeltaPos:
			case EngineNames::CommandName::Light_SetColor:
			case EngineNames::CommandName::Audio_Register:
			case EngineNames::CommandName::Audio_RegisterNotification:
			case EngineNames::CommandName::Audio_Unregister:
			case EngineNames::CommandName::Audio_PostEvent:
			case EngineNames::CommandName::Audio_SetState:
			case EngineNames::CommandName::Audio_SetDefaultListener:
			case EngineNames::CommandName::Audio_SetListener:
			case EngineNames::CommandName::Audio_SendPluginGameData:
			case EngineNames::CommandName::Wav_Fill:
			case EngineNames::CommandName::File_Load:
			case EngineNames::CommandName::SetMessage:
			case EngineNames::CommandName::ComPackage:
			case EngineNames::CommandName::UNINITIALIZED:
			default:
				break;
			}

			delete info->pCookie;
		}
	}
	
	void AudioManager::RenderAudio()
	{
		AK::SoundEngine::RenderAudio();
	}


	bool AudioManager::LoadGlobalBanks()
	{
		poIO->SetBasePath(UTWWISE::FILEPATH::GENERATED_SOUND_BANKS.c_str());
		AK::StreamMgr::SetCurrentLanguage(AKTEXT("English(US)"));

		AKRESULT eResult = AK::SoundEngine::LoadBank(AK::BANKS::INIT);
		assert(eResult == AK_Success);

		eResult = AK::SoundEngine::LoadBank(AK::BANKS::PERCUSSION);
		assert(eResult == AK_Success);

		eResult = AK::SoundEngine::LoadBank(AK::BANKS::MODULAR);
		assert(eResult == AK_Success);

		//AkUniqueID percussionEvent = AK::EVENTS::PERCUSSION;
		//eResult = AK::SoundEngine::PrepareEvent(AK::SoundEngine::Preparation_Load, &percussionEvent, (AkUInt32)1);
		//assert(eResult == AK_Success);

		return true;
	}

	bool AudioManager::LoadPluginInfo()
	{
		pugi::xml_document doc;

		pugi::xml_parse_result result = doc.load_file(UTWWISE::FILEPATH::INFOFILE::SOUND_BANK_INFO.c_str());
		if (result)
		{
			pugi::xml_node busses = doc.first_child().child("SoundBanks").first_child().child("Busses");
			if (busses)
			{
				for (pugi::xml_node_iterator it = busses.begin(); it != busses.end(); ++it)
				{
					pugi::xml_node childNode = it->child("PluginRefs");
					if (childNode)
					{
						childNode = childNode.child("ShareSets").child("PluginRef");
						if (childNode)
						{
							assert(this->poPluginInfo_SA == nullptr);

							this->poPluginInfo_SA = new AudioPluginInfo(
								SpectralAnalysisConfig::PluginID
								, it->attribute("Id").as_uint()
								, AkPluginType::AkPluginTypeEffect
								, SpectralAnalysisConfig::CompanyID
							);
						}
					}
				}
			}
		}

		if (this->poPluginInfo_SA)
		{
			this->poObjectManager->SetGlobalPluginInfo(this->poPluginInfo_SA);
			this->poObjectManager->SetGlobalRequestedPluginData(RequestedData::All, RequestedChannel::All);
			return true;
		}
		else
		{
			return false;
		}
	}

	void AudioManager::TerminateEngine()
	{
		
	#ifndef AK_OPTIMIZED
		AK::Comm::Term();			// Terminate Communication
	#endif // !AK_OPTIMIZED

		AK::MusicEngine::Term();	// Terminate Music Engine
		AK::SoundEngine::Term();	// Terminate Sound Engine
		
		// Terminate streaming device and streaming manager
		poIO->Term();	
		delete poIO;
		if (AK::IAkStreamMgr::Get())
		{
			AK::IAkStreamMgr::Get()->Destroy();
		}

		AK::MemoryMgr::Term();		// Terminate memory manager
	}

	AudioManager* AudioManager::Instance()
	{
		static AudioManager instance;

		return &instance;
	}


	/// ----------------------------------------------------------------------------
	/// ----------------------------------------------------------------------------
	/// ObjectManager methods below ------------------------------------------------
	/// ----------------------------------------------------------------------------
	/// ----------------------------------------------------------------------------

	AudioNode* AudioManager::CreateAudioNode(const char* debugName) { return Instance()->poObjectManager->CreateAudioNode(debugName); }

	void AudioManager::RemoveAudioNode(AudioNode& node) { Instance()->poObjectManager->RemoveAudioNode(node); }

	const AudioPoster*& AudioManager::GetPoster() { return Instance()->poObjectManager->GetPoster(); }

	void AudioManager::PostGlobalEvent(AkUniqueID in_eventID, AkUInt32 in_uFlags, AkCallbackFunc in_pfnCallback, UTWWISE::CallbackCookies cookieType, AkUInt32 in_cExternals, AkExternalSourceInfo* in_pExternalSources, AkPlayingID in_PlayingID)
	{
		Instance()->poObjectManager->PostGlobalEvent(in_eventID, in_uFlags, in_pfnCallback, cookieType, in_cExternals, in_pExternalSources, in_PlayingID);
	}

	void AudioManager::SetGlobalState(AkUniqueID stateGroupID, AkUniqueID state) { Instance()->poObjectManager->SetGlobalState(stateGroupID, state); }

	void AudioManager::AddGlobalSpectralRequest(SpectralRequest* req)
	{
		assert(req);
		Instance()->poObjectManager->AddGlobalSpectralRequest(*req);
	}

	/// ----------------------------------------------------------------------------
	/// ----------------------------------------------------------------------------
	/// Wwise initialization methods below -----------------------------------------
	/// ----------------------------------------------------------------------------
	/// ----------------------------------------------------------------------------

	bool AudioManager::InitWwise()
	{
		bool res = InitAKMemManager();
		assert(res);

		res = InitAKStreamManager();
		assert(res);

		res = InitAKSoundEngine();
		assert(res);

		res = InitAKMusicEngine();
		assert(res);

		//res = InitAKSpatialAudio();
		//assert(res);

	#ifndef AK_OPTIMIZED
		res = InitAKComs();
		assert(res);
	#endif

		return res;
	}

	bool AudioManager::InitAKMemManager()
	{
		AkMemSettings memSettings;
		AK::MemoryMgr::GetDefaultSettings(memSettings);

		if (AK::MemoryMgr::Init(&memSettings) != AK_Success)
		{
			assert(!"Could not create the memory manager.");
			return false;
		}
		return true;
	}

	bool AudioManager::InitAKStreamManager()
	{
		AkStreamMgrSettings stmSettings;
		AK::StreamMgr::GetDefaultSettings(stmSettings);

		// Customize the Stream Manager settings here.
		if (!AK::StreamMgr::Create(stmSettings))
		{
			assert(!"Could not create the Streaming Manager");
			return false;
		}

		//
		// Create a streaming device.
		// Note that you can override the default low-level I/O module with your own. 
		//
		AkDeviceSettings deviceSettings;
		AK::StreamMgr::GetDefaultDeviceSettings(deviceSettings);

		// Customize the streaming device settings here.
		poIO = new CAkDefaultIOHookDeferred();
		// CAkFilePackageLowLevelIODeferred::Init() creates a streaming device
		// in the Stream Manager, and registers itself as the File Location Resolver.
		if (poIO->Init(deviceSettings) != AK_Success)
		{
			assert(!"Could not create the streaming device and Low-Level I/O system");
			return false;
		}

		return true;
	}

	bool AudioManager::InitAKSoundEngine()
	{
		AkInitSettings initSettings;
	#ifndef AK_OPTIMIZED
		initSettings.uMonitorQueuePoolSize = AK_UINT_MAX;
	#endif // !AK_OPTIMIZED

		AkPlatformInitSettings platformInitSettings;
		AK::SoundEngine::GetDefaultInitSettings(initSettings);
		AK::SoundEngine::GetDefaultPlatformInitSettings(platformInitSettings);

		if (AK::SoundEngine::Init(&initSettings, &platformInitSettings) != AK_Success)
		{
			assert(!"Could not initialize the Sound Engine.");
			return false;
		}

		return true;
	}

	bool AudioManager::InitAKMusicEngine()
	{
		AkMusicSettings musicInit;
		AK::MusicEngine::GetDefaultInitSettings(musicInit);

		if (AK::MusicEngine::Init(&musicInit) != AK_Success)
		{
			assert(!"Could not initialize the Music Engine.");
			return false;
		}

		return true;
	}

	bool AudioManager::InitAKSpatialAudio()
	{
		AkSpatialAudioInitSettings settings;
		if (AK::SpatialAudio::Init(settings) != AK_Success)
		{
			assert(!"Could not initialize the Spatial Audio.");
			return false;
		}

		return true;
	}

	bool AudioManager::InitAKComs()
	{
	#ifndef AK_OPTIMIZED

		AkCommSettings commSettings;
		AK::Comm::GetDefaultInitSettings(commSettings);
		if (AK::Comm::Init(commSettings) != AK_Success)
		{
			assert(!"Could not initialize communication.");
			return false;
		}

	#endif // AK_OPTIMIZED

		return true;
	}
}