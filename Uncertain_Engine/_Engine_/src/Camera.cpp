#include "Camera.h"
#include "CamController.h"
#include <assert.h>
#include <math.h>
#include "DXApp.h"

Camera::Camera()
	: poController(nullptr), aspectRatio(0), farDist(0), fovy(0), nearDist(0)
{}

Camera::~Camera()
{
	delete poController;
}

void Camera::SetToDefault(int width, int height)
{
	ALIGN_UNUSED_VAR(width);
	ALIGN_UNUSED_VAR(height);
	this->poController = new CamController(this);
	//setPerspective(45, width / (float)height, 1.0f, 1000.0f);
	setPerspective(45, 3.0f / 2.0f, 1.0f, 1000.0f);
	setOrientAndPosition(Vec3(0.0f, 1.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, -6.5f));
}

void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
{
	this->aspectRatio = Aspect;
	this->fovy = Fovy;
	this->nearDist = NearDist;
	this->farDist = FarDist;
};


void Camera::setOrientAndPosition(const Vec3 &inUp, const Vec3 &inLookAt, const Vec3 &inPos)
{
	// Remember the up, dir and right are unit length, and are perpendicular.
	// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
	// Make sure that all vectors are unit vectors.

	this->vLookAt = inLookAt;
	this->vDir = -(inLookAt - inPos); // Right-Hand camera: vDir is flipped
	this->vDir.norm();   

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross(this->vDir);
	this->vRight.norm();

	this->vUp = this->vDir.cross(this->vRight);
	this->vUp.norm();

	this->vPos = inPos;
};


// The projection matrix 
void Camera::privUpdateProjectionMatrix(void)
{
	float d = 1 / tanf(fovy/2);

	this->projMatrix[m0] = d / aspectRatio;
	this->projMatrix[m1] = 0.0f;
	this->projMatrix[m2] = 0.0f;
	this->projMatrix[m3] = 0.0f;

	this->projMatrix[m4] = 0.0f;
	this->projMatrix[m5] = d;
	this->projMatrix[m6] = 0.0f;
	this->projMatrix[m7] = 0.0f;

	this->projMatrix[m8] = 0.0f;
	this->projMatrix[m9] = 0.0f;
	this->projMatrix[m10] = -(this->farDist + this->nearDist) / (this->farDist - this->nearDist);
	this->projMatrix[m11] = -1.0f; 

	this->projMatrix[m12] = 0.0f;
	this->projMatrix[m13] = 0.0f;
	this->projMatrix[m14] = (-2.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
	this->projMatrix[m15] = 0.0f;

	// Converting from OpenGL-style NDC of [-1,1] to DX's [0,1].  See note: https://anteru.net/blog/2011/12/27/1830/
	// (Note: NDCConvert should be precomputed once and stored for reuse)
	Mat4 B(Mat4::Trans::XYZ, 0.0f, 0.0f, 1.0f); 
	Mat4 S(Mat4::Scale::XYZ, 1.0f, 1.0f, 0.5f);
	Mat4 NDCConvert = B * S;

	projMatrix = projMatrix * NDCConvert;
};


void Camera::privUpdateViewMatrix(void)
{
	// This functions assumes the your vUp, vRight, vDir are still unit
	// And perpendicular to each other

	// Set for DX Right-handed space
	this->viewMatrix[m0] = this->vRight[x];
	this->viewMatrix[m1] = this->vUp[x];
	this->viewMatrix[m2] = this->vDir[x];
	this->viewMatrix[m3] = 0.0f;

	this->viewMatrix[m4] = this->vRight[y];
	this->viewMatrix[m5] = this->vUp[y];
	this->viewMatrix[m6] = this->vDir[y];
	this->viewMatrix[m7] = 0.0f;

	this->viewMatrix[m8] = this->vRight[z];
	this->viewMatrix[m9] = this->vUp[z];
	this->viewMatrix[m10] = this->vDir[z];
	this->viewMatrix[m11] = 0.0f;

	// Apply R^t( -Pos ). Use dot-product with the basis vectors
	this->viewMatrix[m12] = -vPos.dot(vRight);
	this->viewMatrix[m13] = -vPos.dot(vUp);
	this->viewMatrix[m14] = -vPos.dot(vDir);
	this->viewMatrix[m15] = 1.0f;
};


void Camera::updateCamera(_Time frameTime)
{
	ALIGN_UNUSED_VAR(frameTime);

	this->poController->Update();

	// update the projection matrix
	this->privUpdateProjectionMatrix();

	// update the view matrix
	this->privUpdateViewMatrix();

//#ifdef CAMERA_DEBUG
//	DBOUT("Camera Up: " << this->vUp.X() << "f, " << this->vUp.Y() << "f, " << this->vUp.Z() << "f\n");
//	DBOUT("Camera LookAt: " << this->vLookAt.X() << "f, " << this->vLookAt.Y() << "f, " << this->vLookAt.Z() << "f\n");
//	DBOUT("Camera Position: " << this->vPos.X() << "f, " << this->vPos.Y() << "f, " << this->vPos.Z() << "f\n");
//#endif // CAMERA_DEBUG
}



void Camera::TranslateLeftRight(float delta)
{
	vPos += vRight * delta;
}

void Camera::TranslateFwdBack(float delta)
{
	vPos -= vDir * delta;
}

void Camera::TiltUpDown(float ang)
{
	Mat4 Rot = Mat4(Mat4::Rot::AxisAngle, vRight, ang);
	vDir = vDir * Rot;
	vUp = vUp * Rot;
	setOrientAndPosition(vUp, vPos - vDir, vPos);
}

void Camera::TurnLeftRight(float ang)
{
	Mat4 Rot = Mat4(Mat4::Rot1::Y, ang);
	vDir = vDir * Rot;
	vUp = vUp * Rot;
	setOrientAndPosition(vUp, vPos - vDir, vPos);
}


Mat4 & Camera::getViewMatrix(void)
{
	return this->viewMatrix;
}

Mat4 & Camera::getProjMatrix(void)
{
	return this->projMatrix;
}

void Camera::getPos(Vec3 &outPos) const
{
	outPos = this->vPos;
}

void  Camera::getDir(Vec3 &outDir) const
{
	outDir = this->vDir;
}

void  Camera::getUp(Vec3 &outUp) const
{
	outUp = this->vUp;
}

void Camera::getLookAt(Vec3 &outLookAt) const
{
	outLookAt = this->vLookAt;
}

void Camera::getRight(Vec3 &outRight) const
{
	outRight = this->vRight;
}



#ifdef CAMERA_DEBUG
	void Camera::PrintCamera_DEBUG()
	{
		//DBOUT("Camera Up: " << this->vUp.X() << "f, " << this->vUp.Y() << "f, " << this->vUp.Z() << "f\n");
		//DBOUT("Camera LookAt: " << this->vLookAt.X() << "f, " << this->vLookAt.Y() << "f, " << this->vLookAt.Z() << "f\n");
		//DBOUT("Camera Position: " << this->vPos.X() << "f, " << this->vPos.Y() << "f, " << this->vPos.Z() << "f\n");
		DBOUT("{ Vec3(" << this->vPos.X() << "f, " << this->vPos.Y() - 2.0f << "f, " << this->vPos.Z() << "f) },\n");
	}
#else

	void Camera::PrintCamera_DEBUG(){}

#endif // CAMERA_DEBUG