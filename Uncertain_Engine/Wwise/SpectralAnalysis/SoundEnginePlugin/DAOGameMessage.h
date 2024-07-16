#pragma once

#include < AK/SoundEngine/Common/AkCommonDefs.h >
#include "SpectralDAO.h"

class DAOGameMessage
{
public:
	enum class Type : AkUInt16
	{
		Uninitialized			 = 0,
		Register_Game_Spectral	 = 1,
		Unregister_Game_Spectral = 2
	};

	DAOGameMessage() = delete;
	DAOGameMessage(const DAOGameMessage&) = delete;
	DAOGameMessage& operator=(const DAOGameMessage&) = delete;
	virtual ~DAOGameMessage() = default;

	DAOGameMessage(Type t, AkGameObjectID gameObjID)
		: mType(t), mGameObjID(gameObjID)
	{ }

	Type GetType() const { return mType; }
	AkGameObjectID GetGameObjID() const { return mGameObjID; }

protected:
	Type mType;
	AkGameObjectID mGameObjID;
};

class DAOM_Register_Spectral : public DAOGameMessage
{
public:
	DAOM_Register_Spectral(AkGameObjectID gameObjID, SpectralDAO* dao)
	: DAOGameMessage(Type::Register_Game_Spectral, gameObjID)
	, pDAO(dao)
	{}

	SpectralDAO* GetDAO() const { return pDAO; }

private:
	SpectralDAO* pDAO;

};

