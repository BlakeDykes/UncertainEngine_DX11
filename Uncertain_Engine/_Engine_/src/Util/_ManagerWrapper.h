#pragma once

#include "GameObject/GameObjectManager.h"
#include "Shader/ShaderManager.h"
#include "Model/ModelManager.h"
#include "Texture/TextureManager.h"
#include "Lighting/SceneLightManager.h"
#include "Material/MaterialManager.h"
#include "GraphicsObject/GraphicsObjectManager.h"
#include "Reflection/ReflectionManager.h"
#include "_Time.h"
#include "Util.h"
#include "CommandQueueManager.h"
#include "Audio/AudioThread/AudioManager.h"
#include "ThreadManager.h"
#include "Audio/GameThread/RTPC/RTPCManager.h"
#include "File/FileThread/FileThread.h"

#include <d3d11.h>

class ManagerWrapper
{
public:
	static void PreUpdateEvents(_Time time);
	static void Update(_Time time);
	static void Render(ID3D11DeviceContext* context);
	static void Initialize(ID3D11Device*& device, ID3D11RasterizerState* rs_default);
	static void Shutdown();
};