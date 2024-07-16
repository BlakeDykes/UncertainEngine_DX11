#include "AudioObjectManager.h"
#include "DLinkManager.h"
#include "AudioNode.h"
#include "AudioPoster.h"
#include "AudioObject.h"
#include "DLinkIterator.h"

namespace Uncertain
{
	AudioObjectManager* AudioObjectManager::instance = nullptr;

	AudioNode* AudioObjectManager::CreateAudioNode(const char* debugName)
	{
		AudioNode* pNode = (AudioNode*)BaseAdd();

	#ifdef _DEBUG
		pNode->Set(debugName, this->poPoster);
	#endif // _DEBUG

		return pNode;
	}

	void AudioObjectManager::RemoveAudioNode(AudioNode& node)
	{
		BaseRemove(node);
	}

	void AudioObjectManager::AddGlobalSpectralRequest(SpectralRequest& req)
	{
		poGlobalAudio->AddSpectralRequest(req);
	}

	void AudioObjectManager::PostGlobalEvent(AkUniqueID in_eventID, AkUInt32 in_uFlags, AkCallbackFunc in_pfnCallback, UTWWISE::CallbackCookies cookieType, AkUInt32 in_cExternals, AkExternalSourceInfo* in_pExternalSources, AkPlayingID in_PlayingID)
	{
		poGlobalAudio->PostEvent(in_eventID, in_uFlags, in_pfnCallback, cookieType, in_cExternals, in_pExternalSources, in_PlayingID);
	}

	void AudioObjectManager::SetGlobalPluginInfo(AudioPluginInfo* info)
	{
		poGlobalAudio->SetPluginInfo(info);
	}

	void AudioObjectManager::SetGlobalRequestedPluginData(RequestedData reqData, RequestedChannel reqChannels)
	{
		poGlobalAudio->SetRequestedPluginData(reqData, reqChannels);
	}

	void AudioObjectManager::SetGlobalState(AkUniqueID groupID, AkUniqueID stateID)
	{
		poPoster->SetState(groupID, stateID);
	}

	void AudioObjectManager::Update()
	{
		DLinkIterator* pIt = (DLinkIterator*)BaseGetIterator();
		AudioNode* pNode = (AudioNode*)pIt->Current();

		//poGlobalAudio->Update();

		while (!pIt->IsDone())
		{
			pNode->Update();
			pNode = (AudioNode*)pIt->Next();
		}
	}

	AudioObjectManager::~AudioObjectManager()
	{
		delete poPoster;

		instance = nullptr;
	}

	AudioObjectManager* AudioObjectManager::Create()
	{
		assert(instance == nullptr);

		instance = new AudioObjectManager();

		instance->poGlobalAudio->AddListener("Global Listener");

		return instance;
	}

	AkGameObjectID tempGlobalObj = 0;
	AudioObjectManager::AudioObjectManager()
		: ManagerBase(*new DLinkManager(), *new DLinkManager())
		, poPoster(new AudioPoster())
	{
		SetReserve(10, 10);
		this->poGlobalAudio = (AudioNode*)BaseAdd();
		poGlobalAudio->Set("Global Audio", poPoster);
	}


	NodeBase* AudioObjectManager::CreateNode()
	{
		return new AudioNode();
	}

	AudioObjectManager* Uncertain::AudioObjectManager::Instance()
	{
		assert(instance != nullptr);
		return instance;
	}
}

