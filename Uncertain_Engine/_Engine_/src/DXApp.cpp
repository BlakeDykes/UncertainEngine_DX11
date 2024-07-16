#include "DXApp.h"
#include <windows.h>
#include <sstream>
#include <assert.h>
#include "d3dUtil.h"

#include "_ManagerWrapper.h"
#include "GraphicsObject.h"
#include "_GameObjectWrapper.h"
#include "_AudioCommandWrapper.h"

#include "ModelNode.h"
#include "MathEngine.h"

#include <AK/Wwise_IDs.h>
#include <AK/SoundEngine/Common/AkCallback.h>

using namespace ThreadFramework;

static const Vec3 CAMPSITE_ORIGIN = Vec3(100.0f, 19.0f, -30.0f);
static const Vec3 BOAT_ORIGIN = Vec3(-15.0f, -0.5f, -18.0f);
static const Vec3 BOAT_START = Vec3(-10.0f, 10.6f, 10.0f);
static const Vec3 CAMPFIRE_ORIGIN = Vec3(99.75f, 19.05f, -31.25f);

static bool hasStarted = false;
static bool hasStopped = false;

static AkUniqueID MusicEvent = AK::EVENTS::MODULAR_PLAY;
//static AkUniqueID MusicEvent = AK::EVENTS::PERCUSSION


void DXApp::InitAudioDemo()
{
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Static Lighting -------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SceneLightObject* pLights = SceneLightManager::CreateSceneLightObj(ShaderName::ColorLight, LightSourceStatus::Active);
	pLights->CreateDirectional(2.0f * Colors::GhostWhite, 0.5f * Colors::GhostWhite, Vec4(0.75f, 0.75f, 0.75f, 1.0f), Vec4(1.0f, -1.0f, 1.0f).getNorm());

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Shaders ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	ShaderNode* pSH_MagLight =
		ShaderManager::CreateShader(md3dImmediateContext, ShaderName::MagnitudeLight);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Textures ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Materials ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	MaterialObject* pMat_Red = MaterialManager::CreateMatObj(Colors::Uncertain::Red, Colors::Uncertain::Red * 0.25f, Vec4(0.5f, 0.15f, 0.15f, 3.0f));

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Models ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	ModelNode* pModel_AudioMag = ModelManager::CreateAudioSpectrum(FFT::MAG_BUFFER_SAMPLES, 10.0f, 0.05f);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Graphic Objects ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GraphicsObject* pGraph_MagAudio = GraphicsObjectManager::CreateGraphicsObj(pModel_AudioMag, pSH_MagLight, pMat_Red);


	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Game Objects ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GameObject* pGO_MagAudio = GameObjectManager::Add(pGraph_MagAudio, "Audio Magnitude");

	pGO_MagAudio->Translate(Vec3(0.0f, -1.75f, 0.0f));


	// Initialize the projection matrix
	mCam.SetToDefault(mClientWidth, mClientHeight);
}


void DXApp::InitDemo()
{

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Static Lighting ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	SceneLightObject* pLights = SceneLightManager::CreateSceneLightObj(ShaderName::ColorLight_1_3_1, LightSourceStatus::Active);
	PhongADS_Directional* pSceneDirLight = pLights->CreateDirectional(0.75f * Colors::GhostWhite, 0.75f * Colors::GhostWhite, Vec4(1.0f, 1.0f, 1.0f), Vec4(-462.855f, 215.134f, -114.118f).getNorm());

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Shaders ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	ShaderNode* pSh_SimpleTex =
		ShaderManager::CreateShader(md3dImmediateContext, ShaderName::SimpleTexture);
	ShaderNode* pSh_BlendedTile =
		ShaderManager::CreateShader(md3dImmediateContext, ShaderName::BlendedTile_1_3_1);
	ShaderNode* pSh_TextLight =
		ShaderManager::CreateShader(md3dImmediateContext, ShaderName::TextureLight_1_3_1);
	ShaderNode* pSH_ColorLight =
		ShaderManager::CreateShader(md3dImmediateContext, ShaderName::ColorLight_1_3_1);

	//FogParams fogParams;
	//fogParams.FogColor = Colors::MidnightBlue;
	//fogParams.Range = 200;
	//fogParams.Start = 100;

	//ShaderManager::SetFog(fogParams);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Textures ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	TextureNode* pT_Skybox = TextureManager::CreateTextureObj(L"../../../Data/_Textures/nightskybox2.png", FileType::PNG);
	TextureNode* pT_Water = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Water_001_Color.jpg", FileType::PNG);
	TextureNode* pT_Tent = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Tent_001_COLOR.png", FileType::PNG);
	TextureNode* pT_Campfire = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Campfire_001_COLOR.png", FileType::PNG);
	TextureNode* pT_Camper = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Camper_001_COLOR.png", FileType::PNG);
	TextureNode* pT_Paladin = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Paladin_001_COLOR.png", FileType::PNG);
	TextureNode* pT_Guard = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Guard_001_COLOR.png", FileType::PNG);
	TextureNode* pT_Boat = TextureManager::CreateTextureObj(L"../../../Data/_Textures/MetalBoat_001_COLOR.png", FileType::PNG);
	TextureNode* pT_Stump = TextureManager::CreateTextureObj(L"../../../Data/_Textures/Stump_001_COLOR.png", FileType::PNG);
	
	std::vector<LPCWSTR> terrainTexts = { 
		L"../../../Data/_Textures/Sand_001_COLOR.jpg", 
		L"../../../Data/_Textures/Grass_001_COLOR.png", 
		L"../../../Data/_Textures/Rock_001_COLOR.jpg", 
		L"../../../Data/_Textures/Snow_001_COLOR.jpg" 
	};
	TextureNode* pT_Terrain = TextureManager::CreateTextureObj(terrainTexts, FileType::PNG);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Materials ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	Vec4 defaultMatColor = .2f * Colors::GhostWhite;
	MaterialObject* pMat_SoftWhite = MaterialManager::CreateMatObj(defaultMatColor, defaultMatColor * 2.0f, Vec4(1.5f, 0.0f, 0.0f, 30.0f));
	MaterialObject* pMat_Tent = MaterialManager::CreateMatObj(Colors::Beige * .4f, Colors::Beige * .25f, Vec4(0.1f, 0.2f, 0.1f, 20.0f));
	MaterialObject* pMat_Transparent = MaterialManager::CreateMatObj(Vec4(0.0f, 0.2f, 1.0f, 0.25), Vec4(0.0f, 0.2f, 0.2f, 0.65), Vec4(0.00f, 0.01f, 0.5f, 20.0f));
	MaterialObject* pMat_Red = MaterialManager::CreateMatObj(Colors::WhiteSmoke * 0.85f, Colors::WhiteSmoke * 0.75f, Vec4(1.0f, 1.0f, 1.0f, 500.0f));
	MaterialObject* pMat_Blue = MaterialManager::CreateMatObj(Colors::WhiteSmoke * 0.85f, Colors::WhiteSmoke * 0.75f, Vec4(1.0f, 1.0f, 1.0f, 500.0f));
	MaterialObject* pMat_Orange = MaterialManager::CreateMatObj(Colors::RosyBrown * 1.0f, Colors::RosyBrown * 0.5f, Vec4(1.0f, 1.0f, 0.0f, 50.0f));
	MaterialObject* pMat_CamperHighlight = MaterialManager::CreateMatObj(Vec4(0.85f, 0.35f, 0.0f), Vec4(0.85f, 0.1f, 0.0f), Vec4(1.0f, 0.02f, 0.0f, 75.0f));
	MaterialObject* pMat_CamperNightHighlight = MaterialManager::CreateMatObj(Vec4(0.85f, 0.35f, 0.0f), Vec4(0.85f, 0.1f, 0.0f), Vec4(1.0f, 0.3f, 0.0f, 400.0f));
	MaterialObject* pMat_Campfire = MaterialManager::CreateMatObj(Vec4(2.0f, 0.35f, 0.0f), Vec4(1.0f, 0.35f, 0.0f), Vec4(2.0f, 1.0f, 0.0, 100.0f));

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Models ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	ModelNode* pModel_Skybox = ModelManager::CreateSkybox(1000.0f, SkyboxTextures::Night);
	ModelNode* pModel_Terrain = ModelManager::CreateTerrain(L"../../../Data/_HeightMaps/hm_mountainlake_2.tga", 125.0f, 75.0f, -10.0f, 250, 250);
	ModelNode* pModel_Tent = ModelManager::CreateModel("../../../Data/_Models/CampingTent_01.azul");
	ModelNode* pModel_Campfire = ModelManager::CreateModel("../../../Data/_Models/Campfire_01.azul");
	ModelNode* pModel_Camper = ModelManager::CreateModel("../../../Data/_Models/Camper_Sitting_01.azul");
	ModelNode* pModel_Stump = ModelManager::CreateModel("../../../Data/_Models/Stump_01.azul");
	ModelNode* pModel_Paladin = ModelManager::CreateModel("../../../Data/_Models/Paladin_01.azul");
	ModelNode* pModel_Guard = ModelManager::CreateModel("../../../Data/_Models/Guard_01.azul");
	ModelNode* pModel_Boat = ModelManager::CreateModel("../../../Data/_Models/MetalBoat_01.azul");
	ModelNode* pModel_Sphere = ModelManager::CreateSphere();
	
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Graphic Objects ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GraphicsObject* pGO_Skybox = GraphicsObjectManager::CreateGraphicsObj(pModel_Skybox, pSh_SimpleTex, nullptr, pT_Skybox);
	GraphicsObject* pGO_Terrain = GraphicsObjectManager::CreateGraphicsObj(pModel_Terrain, pSh_BlendedTile, pMat_SoftWhite, pT_Terrain);
	GraphicsObject* pGO_Tent = GraphicsObjectManager::CreateGraphicsObj(pModel_Tent, pSh_TextLight, pMat_Tent, pT_Tent);
	GraphicsObject* pGO_Campfire = GraphicsObjectManager::CreateGraphicsObj(pModel_Campfire, pSh_TextLight, pMat_Campfire, pT_Campfire);
	GraphicsObject* pGO_Camper = GraphicsObjectManager::CreateGraphicsObj(pModel_Camper, pSh_TextLight, pMat_CamperHighlight, pT_Camper);
	GraphicsObject* pGO_Stump = GraphicsObjectManager::CreateGraphicsObj(pModel_Stump, pSh_TextLight, pMat_Orange, pT_Stump);
	GraphicsObject* pGO_Paladin = GraphicsObjectManager::CreateGraphicsObj(pModel_Paladin, pSh_TextLight, pMat_SoftWhite, pT_Paladin);
	GraphicsObject* pGO_Guard = GraphicsObjectManager::CreateGraphicsObj(pModel_Guard, pSh_TextLight, pMat_CamperNightHighlight, pT_Guard);
	GraphicsObject* pGO_Boat = GraphicsObjectManager::CreateGraphicsObj(pModel_Boat, pSh_TextLight, pMat_SoftWhite, pT_Boat);
	GraphicsObject* pGO_RedSphereLight = GraphicsObjectManager::CreateGraphicsObj(pModel_Sphere, pSH_ColorLight, pMat_Red);
	GraphicsObject* pGO_BlueSphereLight = GraphicsObjectManager::CreateGraphicsObj(pModel_Sphere, pSH_ColorLight, pMat_Blue);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Reflections ----------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	ReflectionPlane* pReflection1 = ReflectionManager::CreateReflectionPlane(
		pSh_TextLight, pMat_Transparent, pT_Water,
		250.0f, 100.0f, 100.0f,
		Vec3(1.0f, 1.0f, 1.0f), Quat(Quat::Special::Identity), Vec3(0.0f, TERRAIN_WATER_Y, 0.0f));

	
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Game Objects ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GameObject* pSkybox = GameObjectManager::Add_Reflected(pReflection1, pGO_Skybox, "Skybox");
	pSkybox->Translate(Vec3(0.0f, 0.0785398f, 0.706858f));
	pSkybox->Rotate(Quat(Quat::Rot3::XYZ, -476.858f, 105.393f, -144.293f));

	GameObjectManager::Add_Reflected(pReflection1, pGO_Terrain, "Terrain");

	GameObject* pTent = GameObjectManager::Add(pGO_Tent, "Tent");
	pTent->Translate(Vec3(102.5f, 18.91f, -29.5f));
	pTent->Rotate(Quat(Quat::Rot3::XYZ, 0.0f, -2.15985f, 0.0f));

	GameObj_Root* pCampfireRoot = (GameObj_Root*)GameObjectManager::Add(nullptr, "Campfire Root", nullptr, GameObject::Type::Root);
	pCampfireRoot->Translate(CAMPFIRE_ORIGIN);

	PhongADS_Point* pFirelight = pLights->CreatePoint(Colors::OrangeRed * .02f, Colors::OrangeRed * 0.5f, Vec4(2.25f, 0.03f, 0.03f, 100.0f), 
		Vec4(0.0f, 0.0f, 0.0f), .5f * Vec4(1.6f, 1.0f, 1.0f), 25.0f);
	GameObject* pCampfire = pCampfireRoot->Attach(pGO_Campfire, pLights, pFirelight, "Light - Campfire");
	pCampfire->Scale(1.35f);

	pLights->CreateSpot(Vec4(10.0f, 0.33f, 0.0f), Vec4(10.0f, 0.01f, 0.0f) * 10.0f, Vec4(1.0f, 1.0f, 1.0f, 5000.0f),
		Vec4(3.0f, 10.0f, 3.0f), 0.5f * Vec4(1.0f, 0.2f, 0.0f), Vec4(-3.0f, -10.0f, 3.0f).getNorm(), 10.0f, 10.0f);

	GameObject* pCamper = GameObjectManager::Add(pGO_Camper, "Camper 1");
	pCamper->Translate(Vec3(100.75f, 19.0f, -30.5f));
	pCamper->Rotate(Quat(Quat::Rot3::XYZ, -1.53153f, -1.80642f, 0.0f));

	GameObject* pStump = GameObjectManager::Add(pGO_Stump, "Stump");
	pStump->Translate(Vec3(98.75f, 19.0f, -29.75f));

	GameObject* pGuard = GameObjectManager::Add(pGO_Guard, "Camper 2");
	pGuard->Translate(Vec3(98.75f, 19.15f, -29.85f));
	pGuard->Rotate(Quat(Quat::Rot3::XYZ, 0.0f, MATH_3PI4, 0.0f));
	
	GameObj_Root* pBoatRoot = (GameObj_Root*)GameObjectManager::Add_Reflected(pReflection1, nullptr, "Boat Root", nullptr, GameObject::Type::Root);
	pBoatRoot->Translate(BOAT_ORIGIN);

	GameObj_Root* pBoatGroup = (GameObj_Root*)pBoatRoot->Attach(nullptr, "Boat Group", GameObject::Type::Root);
	pBoatGroup->Translate(BOAT_START);
	pBoatGroup->Rotate(Quat(Quat::Rot3::XYZ, 0.0f, -MATH_PI2, -0.05f * MATH_PI2));

	pBoatGroup->Attach(pGO_Boat, "Boat");
	GameObject* pPaladin = pBoatGroup->Attach(pGO_Paladin, "Boat driver");
	pPaladin->Translate(Vec3(0.0f, 0.4f, -2.25f));

	PhongADS_Point* pPoint = pLights->CreatePoint(Colors::White * 0.1f, Colors::White * 0.05f, Vec4(0.0f, 0.2f, 0.3f, 20.0f),
		Vec4(-0.6f, 1.4f, 1.4f), .05f * Vec4(0.0f, 0.0f, 0.01f), 5.0f);

	GameObj_Light* pBoatLight1 = pBoatGroup->Attach(pGO_RedSphereLight, pLights, pPoint, "Boat Light 1");
	pBoatLight1->Scale(0.0377883f);
	
	PhongADS_Point* pPoint2 = pLights->CreatePoint(Colors::White * 0.1f, Colors::White * 0.05, Vec4(0.0f, 0.2f, 0.3f, 20.0f),
		Vec4(0.6f, 1.4f, 1.4f), .05f * Vec4(0.0f, 0.0f, 0.01f), 5.0f);

	GameObj_Light* pBoatLight2 = pBoatGroup->Attach(pGO_BlueSphereLight, pLights, pPoint2, "Boat Light 2");
	pBoatLight2->Scale(0.0377883f);

	pBoatRoot->SetDeltaRot(Vec3(0.0f, -0.016f * MATH_PI8, 0.0f));
	pBoatGroup->SetDeltaRot(Vec3(0.0f, -0.0022f * MATH_PI8, 0.0f));
	pBoatGroup->SetDeltaPos(Vec3(0.0f, -0.0025f, 0.0f), FrameModifier::ModType::Sin_Time);

	// Initialize the projection matrix
	mCam.SetToDefault(mClientWidth, mClientHeight);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	// ---------- Audio ---------------------------------------------------------------------------------------------------------------------------------------------------------
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	/*RTPCC_LightParam* rtpcFireDiffuse = */ RTPCManager::Subscribe(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS, pFirelight, LightParameters::Diffuse, (Vec3)pFirelight->Diffuse, (Vec3)(pFirelight->Diffuse * 25.0f));
	/*RTPCC_LightParam* rtpcFireAmbient = */ RTPCManager::Subscribe(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS, pFirelight, LightParameters::Ambient, (Vec3)pFirelight->Ambient, (Vec3)(pFirelight->Ambient * 15.0f));
	/*RTPCC_LightParam* rtpcSceneDir    =  */RTPCManager::Subscribe(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS, pSceneDirLight, LightParameters::Ambient, (Vec3)pSceneDirLight->Ambient, (Vec3)(pSceneDirLight->Ambient * 0.009f));
}

void DXApp::UpdateScene()
{
	mTimer.Tic();

	ManagerWrapper::PreUpdateEvents(mFrameTime);


#ifdef _DEBUG
	DEBUG_PreUpdateEvents();
#endif // _DEBUG

	mCam.updateCamera(mFrameTime);
	ManagerWrapper::Update(mFrameTime);
	

#ifdef _DEBUG
	DEBUG_PostUpdateEvents();
#endif // _DEBUG

}

void DXApp::DrawScene()
{
	// Clear the back buffer
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, VasA(BackgroundColor));
	// Clear the depth stencil;
	md3dImmediateContext->ClearDepthStencilView(mpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ------- View/Proj --------
	
	ManagerWrapper::Render(md3dImmediateContext);

	// Switches the display to show the now-finished back-buffer
	mSwapChain->Present(SyncInterval, 0);
	mFrameTime = mTimer.Toc();
}

DXApp::DXApp()
	: mhMainWnd(nullptr), BackgroundColor(), SyncInterval(1),
	md3dDevice(nullptr), md3dImmediateContext(nullptr), mSwapChain(nullptr),
	mRenderTargetView(nullptr), mpDepthStencilView(nullptr), mRasterDesc(nullptr),
	mRasterState(nullptr), mCam(), mTimer(), mFrameTime(), mMainWndCaption(),
	mClientWidth(0), mClientHeight(0),
	pControlObj1(nullptr), pControlObj2(nullptr)
{
}

void DXApp::protCreate(HWND hwnd)
{
	mRasterDesc = new D3D11_RASTERIZER_DESC();
	
	assert(hwnd);
	mhMainWnd = hwnd;

	BackgroundColor = Colors::Uncertain::Black;

	md3dDevice = nullptr;
	md3dImmediateContext = nullptr;
	mSwapChain = nullptr;
	mRenderTargetView = nullptr;

	// Get window data through the window handle
	RECT rc;
	BOOL err = GetClientRect(mhMainWnd, &rc);  // Seriously MS: Redifining BOOL as int? Confusing much?
	assert(err);

	// get width/hight
	mClientWidth = rc.right - rc.left;
	mClientHeight = rc.bottom - rc.top;

	// Get window caption
	const int MAX_LABEL_LENGTH = 100; // probably overkill...
	CHAR str[MAX_LABEL_LENGTH];
	GetWindowText(mhMainWnd, str, MAX_LABEL_LENGTH);
	mMainWndCaption = str;

	// Initialize DX11
	this->InitDirect3D();

	ManagerWrapper::Initialize(md3dDevice, mRasterState);

	// Demo initialization
	//this->InitDemo();
	this->InitAudioDemo();

}

void DXApp::InitDirect3D()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// This is a *greatly* simplified process to create a DX device and context:
	// We force the use of DX11 feature level since that's what CDM labs are limited to.
	// For real-life applications would need to test what's the best feature level and act accordingly
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &md3dDevice, nullptr, &md3dImmediateContext);
	assert(SUCCEEDED(hr));

	// Now we obtain the associated DXGIfactory1 with our device 
	// Many steps...
	IDXGIDevice* dxgiDevice = nullptr;
	hr = md3dDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
	assert(SUCCEEDED(hr));

	IDXGIAdapter* adapter = nullptr;
	hr = dxgiDevice->GetAdapter(&adapter);
	assert(SUCCEEDED(hr));

	IDXGIFactory1* dxgiFactory1 = nullptr;
	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory1));
	assert(SUCCEEDED(hr));
	// See also note on weird stuff with factories and swap chains (1s and 2s)
	// https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

	// We are done with these now...
	ReleaseAndDeleteCOMobject(adapter);
	ReleaseAndDeleteCOMobject(dxgiDevice);

	// Controls MSAA option:
	// - 4x count level garanteed for all DX11 
	// - MUST be the same for depth buffer!
	// - We _need_ to work with the depth buffer because reasons... (see below)
	DXGI_SAMPLE_DESC sampDesc;
	sampDesc.Count = 1;
	sampDesc.Quality = static_cast<UINT>(D3D11_CENTER_MULTISAMPLE_PATTERN);  // MS: what's with the type mismtach?

	DXGI_MODE_DESC buffdesc;				// https://msdn.microsoft.com/en-us/library/windows/desktop/bb173064(v=vs.85).aspx
	ZeroMemory(&buffdesc, sizeof(buffdesc));
	buffdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Next we create a swap chain. 
	// Useful thread: http://stackoverflow.com/questions/27270504/directx-creating-the-swapchain
	// Note that this is for a DirectX 11.0: in a real app, we should test the feature levels and act accordingly

	DXGI_SWAP_CHAIN_DESC sd;				// See MSDN: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173075(v=vs.85).aspx
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;						// Much confusion about this number... see http://www.gamedev.net/topic/633807-swap-chain-buffer-count/
	sd.BufferDesc = buffdesc;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = mhMainWnd;
	sd.SampleDesc = sampDesc;
	sd.Windowed = TRUE;

	hr = dxgiFactory1->CreateSwapChain(md3dDevice, &sd, &mSwapChain);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(dxgiFactory1);

	// Create a render target view		https://msdn.microsoft.com/en-us/library/windows/desktop/ff476582(v=vs.85).aspx
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	assert(SUCCEEDED(hr));;

	hr = md3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &mRenderTargetView);
	ReleaseAndDeleteCOMobject(pBackBuffer);
	assert(SUCCEEDED(hr));

	/**********************************************************/

	// First we fix what it means for triangles to be front facing.
	// Requires setting a whole new rasterizer state
	//*
	mRasterDesc->FillMode = D3D11_FILL_SOLID;
	mRasterDesc->CullMode = D3D11_CULL_BACK;
	mRasterDesc->FrontCounterClockwise = true; // true for RH forward facing
	mRasterDesc->DepthBias = 0;
	mRasterDesc->SlopeScaledDepthBias = 0.0f;
	mRasterDesc->DepthBiasClamp = 0.0f;
	mRasterDesc->DepthClipEnable = true;
	mRasterDesc->ScissorEnable = false;
	mRasterDesc->MultisampleEnable = true;  // Does not in fact turn on/off multisample: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476198(v=vs.85).aspx
	mRasterDesc->AntialiasedLineEnable = false;

	md3dDevice->CreateRasterizerState(mRasterDesc, &mRasterState);

	md3dImmediateContext->RSSetState(mRasterState);
	//*/

	// We must turn on the abilty to process depth during rendering.
	// Done through depth stencils (see https://msdn.microsoft.com/en-us/library/windows/desktop/bb205074(v=vs.85).aspx)
	// Below is a simplified version
	//*
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = mClientWidth;
	descDepth.Height = mClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = sampDesc;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	ID3D11Texture2D* pDepthStencil;
	hr = md3dDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
	assert(SUCCEEDED(hr));

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;;

	hr = md3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mpDepthStencilView);
	assert(SUCCEEDED(hr));
	ReleaseAndDeleteCOMobject(pDepthStencil);
	//*/

	/**********************************************************/

	//md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, nullptr);  // to use without depth stencil
	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mpDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)mClientWidth;
	vp.Height = (FLOAT)mClientHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	md3dImmediateContext->RSSetViewports(1, &vp);
}

void DXApp::protDestroy()
{

	ManagerWrapper::Shutdown();

	ReleaseAndDeleteCOMobject(mRasterState);
	delete mRasterDesc;

	ReleaseAndDeleteCOMobject(mpDepthStencilView);
	ReleaseAndDeleteCOMobject(mRenderTargetView);
	ReleaseAndDeleteCOMobject(mSwapChain);
	ReleaseAndDeleteCOMobject(md3dImmediateContext);

	// Must be done BEFORE the device is released
	ReportLiveDXObjects();		// See http://masterkenth.com/directx-leak-debugging/

	ReleaseAndDeleteCOMobject(md3dDevice);
}

// See http://masterkenth.com/directx-leak-debugging/
void DXApp::ReportLiveDXObjects()
{
#ifdef _DEBUG
	HRESULT hr = S_OK;

	// Now we set up the Debug interface, to be queried on shutdown
	ID3D11Debug* debugDev;
	hr = md3dDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&debugDev));

	debugDev->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	ReleaseAndDeleteCOMobject(debugDev);
#endif
}

void DXApp::DEBUG_PreUpdateEvents()
{
	if (hasStarted == false && GetKeyPressHack('P'))
	{
		hasStarted = true;
		AudioManager::PostGlobalEvent(
			MusicEvent
			, AK_MusicSyncEntry
			, &AudioManager::CB_MusicSyncEntry
			, UTWWISE::CallbackCookies::RegisterDAO
		);
		
	}
	if (hasStarted == true && GetKeyPressHack('O'))
	{
		hasStarted = false;

		AudioManager::PostGlobalEvent(
			MusicEvent
		);
	}
	if (GetKeyPressHack('1'))
	{
		AudioManager::SetGlobalState(AK::STATES::PERCUSSION::GROUP, AK::STATES::PERCUSSION::STATE::ACTION);
	}
	if (GetKeyPressHack('2'))
	{
		AudioManager::SetGlobalState(AK::STATES::PERCUSSION::GROUP, AK::STATES::PERCUSSION::STATE::CHILL);
	}
	if (GetKeyPressHack('3'))
	{
		AudioManager::SetGlobalState(AK::STATES::PERCUSSION::GROUP, AK::STATES::PERCUSSION::STATE::OMINOUS);
	}
	if (GetKeyPressHack('4'))
	{
		RTPCManager::LerpTo(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS, 0.0f, _Time(_Time::Duration::ONE_SECOND) * 3.0f);
	}
	if (GetKeyPressHack('5'))
	{
		RTPCManager::LerpTo(AK::GAME_PARAMETERS::PERCUSSION_LOWPASS, 85.0, _Time(_Time::Duration::ONE_SECOND) * 3.0f);
	}

#ifdef CAMERA_DEBUG
	if (GetKeyPressHack('F'))
	{
		mCam.PrintCamera_DEBUG();
	}

#endif

#ifdef WIREFRAME_DEBUG
	// Wireframe toggle
	if (GetKeyPressHack('N') && mRasterDesc->FillMode != D3D11_FILL_WIREFRAME)
	{
		ReleaseAndDeleteCOMobject(mRasterState);

		mRasterDesc->FillMode = D3D11_FILL_WIREFRAME;

		md3dDevice->CreateRasterizerState(mRasterDesc, &mRasterState);

		ReflectionManager::SetDefaulttoWireframe(mRasterState);

		md3dImmediateContext->RSSetState(mRasterState);
	}

	// Solid fill toggle
	if (GetKeyPressHack('M') && mRasterDesc->FillMode != D3D11_FILL_SOLID)
	{
		ReleaseAndDeleteCOMobject(mRasterState);

		mRasterDesc->FillMode = D3D11_FILL_SOLID;

		md3dDevice->CreateRasterizerState(mRasterDesc, &mRasterState);

		ReflectionManager::SetDefaulttoSolidFill(mRasterState);

		md3dImmediateContext->RSSetState(mRasterState);
	}

#endif // WIREFRAME_DEBUG

#ifdef CONTROL_OBJ1_DEBUG
	if (GetKeyPressHack('I'))
	{
		pControlObj1->Translate(Vect(0.0f, 0.0f, CONTROL_OBJ_TRANS_SPEED));
	}
	if (GetKeyPressHack('J'))
	{
		pControlObj1->Translate(Vect(CONTROL_OBJ_TRANS_SPEED, 0.0f, 0.0f));
	}
	if (GetKeyPressHack('K'))
	{
		pControlObj1->Translate(Vect(0.0f, 0.0f, -1.0f * CONTROL_OBJ_TRANS_SPEED));
	}
	if (GetKeyPressHack('L'))
	{
		pControlObj1->Translate(Vect(-1.0f * CONTROL_OBJ_TRANS_SPEED, 0.0f, 0.0f));
	}
	if (GetKeyPressHack('U'))
	{
		pControlObj1->Translate(Vect(0.0f, CONTROL_OBJ_TRANS_SPEED, 0.0f));
	}
	if (GetKeyPressHack('O'))
	{
		pControlObj1->Translate(Vect(0.0f, -1.0f * CONTROL_OBJ_TRANS_SPEED, 0.0f));
	}
	if (GetKeyPressHack('7'))
	{
		pControlObj1->Scale(CONTROL_OBJ_SCALE_DOWN_SPEED);
	}
	if (GetKeyPressHack('8'))
	{
		pControlObj1->Scale(CONTROL_OBJ_SCALE_UP_SPEED);
	}
	if (GetKeyPressHack('9'))
	{
		pControlObj1->Rotate(Vect(0.0f, CONTROL_OBJ_ROT_SPEED, 0.0f));
	}
	if (GetKeyPressHack('0'))
	{
		pControlObj1->Rotate(Vect(0.0f, -1.0f * CONTROL_OBJ_ROT_SPEED, 0.0f));
	}
	if (GetKeyPressHack('3'))
	{
		pControlObj1->Rotate(Vect(CONTROL_OBJ_ROT_SPEED, 0.0f, 0.0f));
	}
	if (GetKeyPressHack('4'))
	{
		pControlObj1->Rotate(Vect(-1.0f * CONTROL_OBJ_ROT_SPEED, 0.0f, 0.0f));
	}
	if (GetKeyPressHack('5'))
	{
		pControlObj1->Rotate(Vect(0.0f, 0.0f, CONTROL_OBJ_ROT_SPEED));
	}
	if (GetKeyPressHack('6'))
	{
		pControlObj1->Rotate(Vect(0.0f, 0.0f, -1.0f * CONTROL_OBJ_ROT_SPEED));
	}

#endif // CONTROL_OBJ1_DEBUG

#ifdef CONTROL_OBJ2_DEBUG
	if (GetKeyPressHack('H'))
	{
		pControlObj2->PreventRender_DEBUG();
	}
	if (GetKeyPressHack('G'))
	{
		pControlObj2->AllowRender_DEBUG();
	}
#endif // CONTROL_OBJ2_DEBUG

}

void DXApp::DEBUG_PostUpdateEvents()
{
#ifdef CONTROL_OBJ1_DEBUG
	/*Vect scale = pControlObj1->GetScale();
	Vect rot = pControlObj1->GetRot();
	Vect pos = pControlObj1->GetTrans();

	DBOUT("----\nObj1 Scale: " << scale.X() << ".0f, " << scale.Y() << ".0f, " << scale.Z() << ".0f\n");
	DBOUT("Obj1 Rot: " << rot.X() << ".0f, " << rot.Y() << ".0f, " << rot.Z() << ".0f\n");
	DBOUT("Obj1 Pos: " << pos.X() << ".0f, " << pos.Y() << ".0f, " << pos.Z() << ".0f\n----\n");*/
#endif // CONTROL_OBJ1_DEBUG
}

DXApp& DXApp::Instance()
{
	static DXApp instance;
	return instance;
}

void DXApp::Create(HWND hwnd)
{
	assert(Instance().mhMainWnd == nullptr);
	Instance().protCreate(hwnd);
}

void DXApp::Destroy()
{
	DXApp* inst = &Instance();
	inst->protDestroy();
}

void DXApp::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((_Time::Quotient(mTimer.Toc(), _Time(_Time::Duration::ONE_SECOND)) - timeElapsed) >= 1.0f)
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::ostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << "    "
			<< "FPS: " << fps << "    "
			<< "Frame Time: " << mspf << " (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void DXApp::FrameTick()
{
	DXApp* inst = &Instance();

	inst->mTimer.Tic();
	inst->CalculateFrameStats();

	inst->UpdateScene();
	inst->DrawScene();
}

void DXApp::OnMouseDown(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseUp(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

void DXApp::OnMouseMove(WPARAM btnState, int xval, int yval)
{
	UNREFERENCED_PARAMETER(btnState);
	UNREFERENCED_PARAMETER(xval);
	UNREFERENCED_PARAMETER(yval);
}

// A terrible way to detect a key being pressed-and-released;
bool DXApp::GetKeyPressHack(char c)
{
	bool ans = false;
	while (GetAsyncKeyState(c) & 0x08000)
		ans = true;

	return ans;
}
