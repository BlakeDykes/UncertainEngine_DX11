// DXApp
// Andre Berthiaume, June 2016
// Note: DX 11 SDK https://www.microsoft.com/en-us/download/details.aspx?id=6812
// Note on weird stuff with swap chain (1s and 2s) https://msdn.microsoft.com/en-us/library/windows/desktop/jj863687(v=vs.85).aspx

#ifndef _DXApp
#define _DXApp

#include <d3d11.h>
#include "d3dUtil.h"
#include "_Timer.h"
#include <string>
#include <vector>
#include <Windows.h>
#include <iostream>
#include <sstream>

// New includes for demo
#include "Vec4.h"
#include "Camera.h"

class ShaderObject;
class Model;
class GameObject;
class GameObj_Parent;
class GraphicsObject;

#define DBOUT( s )				\
{								\
	std::ostringstream os_;		\
	os_ << s;					\
	OutputDebugString(os_.str().c_str()); \
}							

#define WIREFRAME_DEBUG
//#define CAMERA_DEBUG

//#define CONTROL_OBJ1_DEBUG
//#define CONTROL_OBJ2_DEBUG

#ifdef CONTROL_OBJ1_DEBUG
#define CONTROL_OBJ_TRANS_SPEED 0.1f			
#define CONTROL_OBJ_ROT_SPEED   0.5f * MATH_PI8	
#define CONTROL_OBJ_SCALE_UP_SPEED 1.01f		
#define CONTROL_OBJ_SCALE_DOWN_SPEED .99f		
#endif // CONTROL_OBJ1_DEBUG

//#define LIGHT_DEBUG

class DXApp : public Align16
{
private:	
	// Main window handle
	HWND      mhMainWnd;

	Vec4 BackgroundColor;
	int SyncInterval = 1;  // 1 for sync to monitor refresh rate, 0 for no sync

	// DX application elements
	ID3D11Device* md3dDevice;					// Connects to the graphics card
	ID3D11DeviceContext* md3dImmediateContext;	// Settings for the GPU to use
	IDXGISwapChain* mSwapChain;					// image buffers used for rendering
	ID3D11RenderTargetView* mRenderTargetView;	// Where to send rendring operations (typically: points to one of the swap buffers)
	ID3D11DepthStencilView* mpDepthStencilView; // Needed to force depth-buffer operations

	D3D11_RASTERIZER_DESC* mRasterDesc;
	ID3D11RasterizerState* mRasterState;

	Camera mCam;

	_Timer mTimer;
	_Time mFrameTime;
	std::string mMainWndCaption;
	int mClientWidth;
	int mClientHeight;

	GameObject* pControlObj1;
	GameObject* pControlObj2;

	DXApp();
	void protCreate(HWND hwnd);
	void protDestroy();

	void InitDemo();
	void InitAudioDemo();

	void InitDirect3D();
	void UpdateScene();
	void DrawScene();
	void CalculateFrameStats();

	// Debug utility
	void ReportLiveDXObjects();
	void DEBUG_PreUpdateEvents();
	void DEBUG_PostUpdateEvents();

	static DXApp& Instance();

public:
	static void Create(HWND hwnd);
	static void Destroy();

	static void FrameTick();

	// overrides for handling mouse input.
	static void OnMouseDown(WPARAM btnState, int xval, int yval);
	static void OnMouseUp(WPARAM btnState, int xval, int yval);
	static void OnMouseMove(WPARAM btnState, int xval, int yval);

	static Camera& GetCamera() { return Instance().mCam; }

	bool GetKeyPressHack(char c);
};

#endif _DXApp