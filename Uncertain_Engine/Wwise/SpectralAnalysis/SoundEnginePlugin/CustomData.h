#pragma once
#include "_def_SA.h"

#include <AK/SoundEngine/Common/AkCommonDefs.h>

enum class CDType : AkUInt8
{
	RegisterDAO,
	UnregisterDAO
};

class CustomData
{
public:
	CustomData() = delete;
	CustomData(const CustomData&) = delete;
	CustomData& operator=(const CustomData&) = delete;
	virtual ~CustomData() = default;

	CustomData(CDType t, AkGameObjectID goid);

	AkGameObjectID GetGameObjID() const { return GOID; }
	CDType GetType() const { return this->Type; }

	virtual AkUInt32 GetThisSize() const = 0;

protected:
	AkGameObjectID GOID;
	CDType Type;
	const char _pad1[3] = { 0 };
};