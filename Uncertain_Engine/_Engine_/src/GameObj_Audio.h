#pragma once

#include <AK/SoundEngine/Common/AkCommonDefs.h>
#include "GameObject.h"

class GameObj_Audio : public GameObject
{
private:
	AkGameObjectID AGOID;

public:
	GameObj_Audio(AkGameObjectID aoid, GraphicsObject* graphicsObject, const char* const name, RenderOrder order);

	virtual ~GameObj_Audio();
};
