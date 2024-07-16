#include "AudioNode.h"
#include "AudioObject.h"
#include "AudioManager.h"
#include "SpectralRequest.h"

namespace Uncertain
{
	AudioNode::AudioNode()
		: pPoster(nullptr), poObject(nullptr), Listeners(), SpecRequests()
	{
	#ifdef _DEBUG
		DebugID = std::string();
	#endif // _DEBUG

	}

	AudioNode::~AudioNode()
	{
		delete poObject;

		for (auto it = Listeners.begin(); it != Listeners.end(); ++it)
		{
			delete it->second;
		}
	}

	void AudioNode::Update()
	{
		poObject->ProcessRequests(SpecRequests.begin(), SpecRequests.end());
	}

	void AudioNode::Set(const char* debugID, AudioPoster* globalPoster)
	{
		pPoster = globalPoster;
		this->poObject = new AudioObject();

		pPoster->Register(*poObject, debugID);

	#ifdef _DEBUG
		DebugID = debugID;
	#endif // _DEBUG
	}

	AkGameObjectID AudioNode::AddListener(const char* debugName, AkUniqueID emitterID)
	{
		AudioObject* pObj = new AudioObject();
		AkGameObjectID id = pObj->GetID();

		if (emitterID == DEFAULT_EMITTER)
		{
			pPoster->SetDefaultListener(id);
		}
		else
		{
			assert(Listeners.find(emitterID) != Listeners.end());
			pPoster->SetListener(emitterID, id);
		}

		Listeners.emplace(std::make_pair(id, pObj));
		pPoster->Register(*pObj, debugName);

		return id;
	}

	void AudioNode::AddSpectralRequest(SpectralRequest& req)
	{
		SpecRequests.push_back(&req);
	}

	void AudioNode::RemoveSpectralRequest(SpectralRequest& req)
	{
		SpecRequests.remove(&req);
	}

	void AudioNode::PostEvent(
		AkUniqueID in_eventID
		, AkUInt32 in_uFlags
		, AkCallbackFunc in_pfnCallback
		, UTWWISE::CallbackCookies cookieType
		, AkUInt32 in_cExternals
		, AkExternalSourceInfo* in_pExternalSources
		, AkPlayingID 	in_PlayingID
	)
	{
		void* pCookie = poObject->GetCallbackCookie(cookieType);

		this->pPoster->PostEvent(in_eventID, poObject->GetID(), in_uFlags,
			in_pfnCallback, pCookie, in_cExternals, in_pExternalSources, in_PlayingID);
	}

	void AudioNode::SendPluginGameData(const void* in_pData, AkUInt32 in_uSizeInBytes)
	{
		this->pPoster->SendPluginGameData(poObject->GetID(), poObject->GetPluginInfo(), in_pData, in_uSizeInBytes);
	}

	void AudioNode::SetAsDefaultListener(AkGameObjectID gameObjID)
	{
		assert(Listeners.find(gameObjID) != Listeners.end());
		pPoster->SetDefaultListener(gameObjID);
	}

	void AudioNode::SetRequestedPluginData(RequestedData reqData, RequestedChannel reqChannel)
	{
		 poObject->SetRequestedPluginData(reqData, reqChannel); 
	}

	void AudioNode::SetPluginInfo(AudioPluginInfo* info)
	{
		poObject->SetPluginInfo(info);
	}

	bool AudioNode::Compare(NodeBase& pNode)
	{
		return this == &pNode;
	}

	void AudioNode::Wash()
	{
		pPoster->Unregister(poObject->GetID());
		delete poObject;

		for (auto it = Listeners.begin(); it != Listeners.end(); ++it)
		{
			pPoster->Unregister(it->second->GetID());
			delete it->second;
		}

		Listeners.clear();
		pPoster = nullptr;
	}

}
