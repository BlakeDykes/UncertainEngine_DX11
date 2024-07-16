#include "DeltaMod.h"

#include "Utils.h"

void ModManager::Destroy()
{
	ModManager* inst = &Instance();

	for (DeltaMod* mod : inst->Mods)
	{
		delete mod;
	}
}

void ModManager::Exchange(DeltaMod*& oldMod, const Vec3& v, FrameModifier::ModType mType)
{
	ModManager* inst = &Instance();

	if (oldMod->GetIndex() != 0)
	{
		oldMod->Modifier = inst->GetFrameModifier(mType);
		oldMod->Delta = v;

	}
	else
	{
		oldMod = new DeltaMod(inst->Mods.size(), v, inst->GetFrameModifier(mType));
		inst->Mods.push_back(oldMod);
	}
}

DeltaMod* ModManager::CreateDeltaMod(const Vec3& v, FrameModifier::ModType mtype)
{
	ModManager* inst = &Instance();

	DeltaMod* mod = new DeltaMod(inst->Mods.size(), v, inst->GetFrameModifier(mtype));
	inst->Mods.push_back(mod);

	return mod;
}

void ModManager::Update(_Time frameTime)
{
	Instance().privUpdate(frameTime);
}

ModManager::ModManager()
	: FMod_Null(), FMod_Default(), FMod_SinTime(), FMod_CosTime(), Mods()
{
	Mods.push_back(new DeltaMod(0, Vec3(0.0f, 0.0f, 0.0f), &FMod_Null));
}

void ModManager::privUpdate(_Time frameTime)
{
	float time = _Time::Quotient(frameTime, _Time(_Time::Duration::ONE_SECOND)) * 0.001f;

	this->FMod_SinTime.Update(time);
	this->FMod_CosTime.Update(time);
}

ModManager& ModManager::Instance()
{
	static ModManager instance;
	return instance;
}

FrameModifier* ModManager::GetFrameModifier(FrameModifier::ModType mType)
{
	switch (mType)
	{
	case FrameModifier::ModType::Null:		return &FMod_Null;
	case FrameModifier::ModType::Sin_Time:	return &FMod_SinTime;
	case FrameModifier::ModType::Cos_Time:	return &FMod_CosTime;

	case FrameModifier::ModType::Default:
	default:								return &FMod_Default;
	}
}
