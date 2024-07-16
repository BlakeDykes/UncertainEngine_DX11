#pragma once

#include "Vec3.h"
#include <vector>
#include "_Time.h"

struct FrameModifier
{
	enum class ModType
	{
		Null,
		Default,
		Sin_Time,
		Cos_Time
	};


	ModType mType;
	float FrameMod;
	float WorkingValue;

	FrameModifier(ModType t)
		: mType(t), FrameMod(1.0f), WorkingValue(0.0f)
	{
	}

	virtual void Update(float mTime) = 0;
	virtual const Vec3 Mod(const Vec3& v) const = 0;

	bool operator==(FrameModifier const& rhs) const { return this->mType == rhs.mType; }
	bool operator!=(FrameModifier const& rhs) const { return this->mType != rhs.mType; }
	bool operator==(ModType const& rhs) const { return this->mType == rhs; }
	bool operator!=(ModType const& rhs) const { return this->mType != rhs; }
};

struct FrameMod_Null : public FrameModifier
{
	FrameMod_Null()
		: FrameModifier(ModType::Null), ZeroVect(0.0f, 0.0f, 0.0f)
	{ }

	Vec3 ZeroVect;

	virtual void Update(float mTime) override { ALIGN_UNUSED_VAR(mTime); };
	virtual const Vec3 Mod(const Vec3& v) const override { ALIGN_UNUSED_VAR(v); return ZeroVect; }
};

struct FrameMod_Default : public FrameModifier
{
	FrameMod_Default()
		: FrameModifier(ModType::Default)
	{
	}

	virtual void Update(float mTime) override { ALIGN_UNUSED_VAR(mTime); };
	virtual const Vec3 Mod(const Vec3& v) const override { return v; }
};

struct FrameMod_SinTime : public FrameModifier
{
	FrameMod_SinTime()
		: FrameModifier(ModType::Sin_Time)
	{
	}

	virtual void Update(float mTime) override { WorkingValue += mTime * .02f; this->FrameMod = sinf(WorkingValue); }
	virtual const Vec3 Mod(const Vec3& v) const override { return v * this->FrameMod; }
};

struct FrameMod_CosTime : FrameModifier
{
	FrameMod_CosTime()
		: FrameModifier(ModType::Cos_Time)
	{
	}

	virtual void Update(float mTime) override { WorkingValue += mTime * .02f; this->FrameMod = cosf(WorkingValue); }
	virtual const Vec3 Mod(const Vec3& v) const override { return v * this->FrameMod; }
};

struct DeltaMod : public Align16
{
	DeltaMod(size_t index, const Vec3& v, FrameModifier* fm)
		: Index(index), Delta(v), Modifier(fm)
	{
	}

	Vec3 GetDeltaMod() const { return Modifier->Mod(Delta); }
	
	FrameModifier::ModType ModType() const { return Modifier->mType; }
	void SetDelta(const Vec3& v) { this->Delta = v; }
	size_t GetIndex() const { return Index; }

private:

	friend class ModManager;
	size_t Index;
	Vec3 Delta;
	FrameModifier* Modifier;
};

class ModManager
{
public:
	static void Destroy();
	static DeltaMod* GetNull() { return Instance().Mods[0]; }
	static void Exchange(DeltaMod*& oldMod, const Vec3& v, FrameModifier::ModType mType);
	static DeltaMod* CreateDeltaMod(const Vec3& v, FrameModifier::ModType mtype);
	static void Update(_Time mTime);

private:
	ModManager();
	void privUpdate(_Time mTime);
	static ModManager& Instance();

	FrameModifier* GetFrameModifier(FrameModifier::ModType mType);

	FrameMod_Null	 FMod_Null;
	FrameMod_Default FMod_Default;
	FrameMod_SinTime FMod_SinTime;
	FrameMod_CosTime FMod_CosTime;

	std::vector<DeltaMod*> Mods;
};
