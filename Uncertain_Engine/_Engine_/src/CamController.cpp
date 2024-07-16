#include "CamController.h"
#include "Camera.h"

CamController::CamController(Camera* cam, float camSpeed, float rotSpeed)
	: mpCam(cam), CamSpeed(camSpeed), RotSpeed(rotSpeed)
{
}

void CamController::Update()
{
	if (GetKeyState('W') & 0x08000)
	{
		mpCam->TranslateFwdBack(CamSpeed);
	}
	else if (GetKeyState('S') & 0x08000)
	{
		mpCam->TranslateFwdBack(-CamSpeed);
	}

	if (GetKeyState('A') & 0x08000)
	{
		mpCam->TranslateLeftRight(-CamSpeed);
	}
	else if (GetKeyState('D') & 0x08000)
	{
		mpCam->TranslateLeftRight(CamSpeed);
	}

	if (GetKeyState(VK_LEFT) & 0x08000)
	{
		mpCam->TurnLeftRight(RotSpeed);
	}
	else if (GetKeyState(VK_RIGHT) & 0x08000)
	{
		mpCam->TurnLeftRight(-RotSpeed);
	}

	if (GetKeyState(VK_UP) & 0x08000)
	{
		mpCam->TiltUpDown(RotSpeed);
	}
	else if (GetKeyState(VK_DOWN) & 0x08000)
	{
		mpCam->TiltUpDown(-RotSpeed);
	}
}
