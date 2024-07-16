#pragma once
#include "_def_SA.h"

#include "CustomData.h"
#include "SpectralDAO.h"

class CD_RegisterDAO : public CustomData
{
public:
	CD_RegisterDAO(SpectralDAO* dao, AkGameObjectID goid)
	: CustomData(CDType::RegisterDAO, goid), pDAO(dao)
	{}

	SpectralDAO* GetDAO() { return pDAO; }

	AkUInt32 GetThisSize() const override { return sizeof(CD_RegisterDAO); }

private:
	SpectralDAO* pDAO;
};

class CD_UnregisterDAO : public CustomData
{
	AkUInt32 GetThisSize() const override { return sizeof(CD_UnregisterDAO); }

public:
	CD_UnregisterDAO(AkGameObjectID goid)
		: CustomData(CDType::UnregisterDAO, goid)
	{}
};