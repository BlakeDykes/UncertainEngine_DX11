#include "RTPCManager.h"
#include "RTPCObject.h"
#include <AK/Wwise_IDs.h>
#include "_RTPCCWrapper.h"

RTPCManager* RTPCManager::instance = nullptr;

Uncertain::RTPCManager::RTPCManager()
	: RTPCs()
{
}

Uncertain::RTPCManager::~RTPCManager()
{
	for (auto it = RTPCs.begin(); it != RTPCs.end(); ++it)
	{
		delete it->second;
	}
}

void Uncertain::RTPCManager::Create()
{
	assert(instance == nullptr);

	instance = new RTPCManager();
	instance->LoadAll();
}

void Uncertain::RTPCManager::Destroy()
{
	delete instance;
	instance = nullptr;
}

RTPCC_LightParam* Uncertain::RTPCManager::Subscribe(AkRtpcID rtpcID, PhongADS_Directional* pLight, LightParameters lParam, const Vec3& min, const Vec3& max)
{
	RTPCManager* inst = Instance();

	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	RTPCC_LP_Dir* pCom = new RTPCC_LP_Dir(pLight, lParam, min, max);

	inst->RTPCs[rtpcID]->Subscribe(pCom);

	return pCom;
}

RTPCC_LightParam* Uncertain::RTPCManager::Subscribe(AkRtpcID rtpcID, PhongADS_Point* pLight, LightParameters lParam, const Vec3& min, const Vec3& max)
{
	RTPCManager* inst = Instance();

	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	RTPCC_LP_Point* pCom = new RTPCC_LP_Point(pLight, lParam, min, max);

	inst->RTPCs[rtpcID]->Subscribe(pCom);

	return pCom;
}

RTPCC_LightParam* Uncertain::RTPCManager::Subscribe(AkRtpcID rtpcID, PhongADS_Spot* pLight, LightParameters lParam, const Vec3& min, const Vec3& max)
{
	RTPCManager* inst = Instance();

	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	RTPCC_LP_Spot* pCom = new RTPCC_LP_Spot(pLight, lParam, min, max);

	inst->RTPCs[rtpcID]->Subscribe(pCom);

	return pCom;
}

void Uncertain::RTPCManager::Set(AkRtpcID rtpcID, AkRtpcValue value)
{
	RTPCManager* inst = Instance();

	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	inst->RTPCs[rtpcID]->Set(value);
}

void Uncertain::RTPCManager::Add(AkRtpcID rtpcID, float value)
{
	RTPCManager* inst = Instance();

	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	inst->RTPCs[rtpcID]->Add(value);
}

void Uncertain::RTPCManager::Reduce(AkRtpcID rtpcID, float value)
{
	RTPCManager* inst = Instance();

	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	inst->RTPCs[rtpcID]->Reduce(value);
}

void Uncertain::RTPCManager::Update()
{
	RTPCManager* inst = Instance();

	for (auto it = inst->RTPCs.begin(); it != inst->RTPCs.end(); ++it)
	{
		it->second->Update(it->first);
	}
}

void Uncertain::RTPCManager::LerpTo(AkRtpcID rtpcID, AkRtpcValue value, _Time totalLerpTime)
{
	RTPCManager* inst = Instance();
	assert(inst->RTPCs.find(rtpcID) != inst->RTPCs.end());

	inst->RTPCs[rtpcID]->LerpToVal(value, totalLerpTime);
}


void Uncertain::RTPCManager::LoadAll()
{
	RTPCs.emplace(std::make_pair(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS, new RTPCObject(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS)));
}

RTPCManager* Uncertain::RTPCManager::Instance()
{
	assert(instance);
	return instance;
}
