#include "GameObj_Audio.h"
#include "CBuffers.h"

GameObj_Audio::GameObj_Audio(AkGameObjectID aoid, GraphicsObject* graphicsObject, const char* const name, RenderOrder order)
	: GameObject(graphicsObject, name, order), AGOID(aoid)
{
}

GameObj_Audio::~GameObj_Audio()
{
}
