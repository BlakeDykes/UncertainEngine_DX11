#ifndef CAMERA_H
#define CAMERA_H

#include "Mat4.h"
#include "Vec3.h"
#include "_Time.h"

class CamController;

class Camera final 
{

public:
	// Default constructor
	Camera();
	~Camera();

	void SetToDefault(int width, int height);

	// Setup on single camera
	void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
	void setOrientAndPosition(const Vec3 &Up_vect, const Vec3 &inLookAt_pt, const Vec3 &pos_pt);

	// update camera system
	void updateCamera(_Time frameTime);

	void TranslateLeftRight(float delta);
	void TranslateFwdBack(float delta);
	void TiltUpDown(float ang);
	void TurnLeftRight(float ang);

	// Get the matrices for rendering
	Mat4& getViewMatrix();
	Mat4& getProjMatrix();

	// accessors
	void getPos(Vec3 &outPos) const;
	void getDir(Vec3 &outDir) const;
	void getUp(Vec3 &outUp) const;
	void getLookAt(Vec3 &outLookAt) const;
	void getRight(Vec3 &outRight) const;

	void PrintCamera_DEBUG();

private:  // methods should never be public
	void privUpdateProjectionMatrix(void);
	void privUpdateViewMatrix(void);


private:  // data  (Keep it private)

	CamController* poController;

	Mat4	projMatrix;
	Mat4	viewMatrix;

	// camera unit vectors (up, dir, right)
	Vec3	vUp;
	Vec3	vDir;
	Vec3	vRight;  // derived by up and dir
	Vec3	vPos;
	Vec3	vLookAt;

	// Define the frustum inputs
	float	nearDist;
	float	farDist;
	float	fovy;  // aka view angle along y axis
	float	aspectRatio;
};

#endif