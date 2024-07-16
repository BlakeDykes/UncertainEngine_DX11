#pragma once

#include "GameObjectManager.h"
#include "ShaderManager.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "SceneLightManager.h"
#include "MaterialManager.h"
#include "GraphicsObjectManager.h"
#include "ReflectionManager.h"
#include "_Time.h"
#include "Util.h"
#include "CommandQueueManager.h"
#include "AudioManager.h"
#include "ThreadManager.h"
#include "RTPCManager.h"
#include "FileThread.h"

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