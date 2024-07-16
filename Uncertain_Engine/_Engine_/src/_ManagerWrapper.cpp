#include "_ManagerWrapper.h"
#include "DXApp.h"

void ManagerWrapper::PreUpdateEvents(_Time time)
{
	ModManager::Update(time);
	RTPCManager::Update();
}

void ManagerWrapper::Update(_Time time)
{
	ReflectionManager::Update(time);
	GameObjectManager::Update();
	SceneLightManager::Update(time);
}

void ManagerWrapper::Render(ID3D11DeviceContext* context)
{
	ShaderManager::SetCameraAndLighting(DXApp::GetCamera());
	GameObjectManager::Draw(context);
	ReflectionManager::RenderReflections(context);
}

void ManagerWrapper::Initialize(ID3D11Device*& device, ID3D11RasterizerState* rs_default)
{
	CommandQueueManager::Create();
	FileThread::Create();

	PromiseFlag audioReady;
	AudioManager::Create(&audioReady);

	RTPCManager::Create();
	GameObjectManager::Create();
	ShaderManager::Create(device);
	TextureManager::Create(device);
	ModelManager::Create(device);
	SceneLightManager::Create(device);
	ReflectionManager::Create(device, rs_default);

	while (!audioReady.IsFlagged()) {}
}

void ManagerWrapper::Shutdown()
{
	ThreadManager::Shutdown();
	RTPCManager::Destroy();
	ShaderManager::DestroyAll();
	TextureManager::DestroyAll();
	ModelManager::DestroyAll();
	GraphicsObjectManager::DestroyAll();
	SceneLightManager::DestroyAll();
	ReflectionManager::Shutdown();
	ModManager::Destroy();
	GameObjectManager::Destroy();

	CommandQueueManager::Destroy();
}

