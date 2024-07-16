#include "Utils.h"

FrameModifier* FrameModManager::GetFrameModifier(FrameModifier::ModType mtype)
{
	switch (mtype)
	{
	case FrameModifier::ModType::Sin_Time:	return &(Instance().FMod_SinTime);
	case FrameModifier::ModType::Cos_Time:	return &(Instance().FMod_CosTime);

	case FrameModifier::ModType::Default:	
	default:								return &(Instance().FMod_Default);
	}
}

void FrameModManager::Update(float frameTime)
{
	Instance().privUpdate(frameTime);
}

FrameModManager::FrameModManager()
	: FMod_Default(), FMod_SinTime(), FMod_CosTime()
{
}

void FrameModManager::privUpdate(float frameTime)
{
	this->FMod_SinTime.Update(frameTime);
	this->FMod_CosTime.Update(frameTime);
}

FrameModManager& FrameModManager::Instance()
{
	static FrameModManager instance;
	return instance;
}
