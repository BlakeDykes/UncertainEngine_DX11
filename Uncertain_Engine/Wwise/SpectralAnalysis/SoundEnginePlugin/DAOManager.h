#pragma once
#include "_def_SA.h"

#include <AK/SoundEngine/Common/IAkPlugin.h>

#include "SpectralDAO.h"
#include <unordered_map>

class FFT;

class DAOManager
{
public:
	DAOManager();
	DAOManager(const DAOManager&) = delete;
	DAOManager& operator =(const DAOManager&) = delete;
	~DAOManager() = default;

	void Setup(AK::IAkPluginMemAlloc* pAlloc, AkAudioFormat& in_rFormat);
	void Destroy(AK::IAkPluginMemAlloc* pAlloc);

	bool HasSubscribers() { return pGameSpectral != nullptr; }
	void ReceiveGameData(AK::IAkEffectPluginContext* pContext);
	void WriteToGame(SpectralDAO* data);

	//bool ShouldProcessChannel(AkUInt8 channelIndex);
	//bool ShouldProcessData(AkUInt8 channel);

private:
	SpectralDAO* pGameSpectral;
	const char _pad1[4] = { 0 };
	AkGameObjectID GameSpectralID;
	//void RecordRequest(SpectralDAO* dao);
	//void RemoveRequest(AkGameObjectID key);

	//AkUInt16* poChannelReqCounts;
	//AkUInt16* poDataReqCounts;
	//AkUInt8   ChannelCount;
};