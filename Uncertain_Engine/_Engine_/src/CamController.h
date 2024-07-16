#pragma once

const static float DEFAULT_CAM_SPEED = 0.25f;
const static float DEFAULT_ROT_SPEED = 0.025f;

class Camera;

class CamController
{
public:
	CamController(Camera* cam, float camSpeed = DEFAULT_CAM_SPEED,
		float rotSpeed = DEFAULT_ROT_SPEED);

	void Update();

private:
	Camera* mpCam;

	float CamSpeed;
	float RotSpeed;



};