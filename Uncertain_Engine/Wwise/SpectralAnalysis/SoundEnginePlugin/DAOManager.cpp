#include "DAOManager.h"
#include "SA_FFT.h"
#include "_Wrapper_CustomData.h"

DAOManager::DAOManager()
	: pGameSpectral(nullptr)
	, GameSpectralID(0)
	//, poChannelReqCounts(nullptr)
	//, poDataReqCounts(nullptr)
{
}

void DAOManager::Setup(AK::IAkPluginMemAlloc* pAlloc, AkAudioFormat& in_rFormat)
{
	AK_UNUSEDVAR(pAlloc);
	AK_UNUSEDVAR(in_rFormat);

	//poChannelReqCounts = static_cast<AkUInt16*>(AK_PLUGIN_ALLOC(pAlloc, (in_rFormat.GetNumChannels() + 1) * sizeof(AkUInt16))); // <-- numchannels + 1 to account for sum calculation
	//poDataReqCounts = static_cast<AkUInt16*>(AK_PLUGIN_ALLOC(pAlloc, 3));
}

void DAOManager::Destroy(AK::IAkPluginMemAlloc* pAlloc)
{
	//AK_PLUGIN_FREE(pAlloc, poChannelReqCounts);
	//AK_PLUGIN_FREE(pAlloc, poDataReqCounts);
}

void DAOManager::ReceiveGameData(AK::IAkEffectPluginContext* pContext)
{
	void* pData;
	AkUInt32 dataSize = 0;

	pContext->GetPluginCustomGameData(pData, dataSize);

	if (pData != NULL)
	{
		CustomData* pBase = static_cast<CustomData*>(pData);
		CDType t = pBase->GetType();

		switch (t)
		{
		case CDType::RegisterDAO:
		{
			CD_RegisterDAO* pDerived = static_cast<CD_RegisterDAO*>(pBase);
			this->pGameSpectral = pDerived->GetDAO();
			this->GameSpectralID = pDerived->GetGameObjID();
			//RecordRequest(pDAO);
			
			break;
		}
		case CDType::UnregisterDAO:
		{
			CD_UnregisterDAO* pDerived = static_cast<CD_UnregisterDAO*>(pBase);
			
			if (GameSpectralID == pDerived->GetGameObjID())
			{
				GameSpectralID = 0;
				pGameSpectral = nullptr;
			}
			//RemoveRequest(id);

			break;
		}
		default:
			break;
		}
	}
}

void DAOManager::WriteToGame(SpectralDAO* pData)
{
	if(pGameSpectral) pGameSpectral->Pull(pData);
}
