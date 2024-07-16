//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"

using namespace Uncertain;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Quat_MatrixTimesQuat, TestConfig::ALL)
{
#if Quat_MatrixTimesQuat

	Quat q1, q2;
	Quat out1, out2;
	// Quat * Matrix 
	Mat4 mxyz, mx, my, mz, mtmp;
	Quat qxyz, qx, qy, qz, qtmp, qxy;

	mx.set(Mat4::Rot1::X, MATH_PI3);
	my.set(Mat4::Rot1::Y, MATH_7PI8);
	mz.set(Mat4::Rot1::Z, MATH_PI2);

	mxyz = mx * my * mz;

	qx.set(Quat::Rot1::X, MATH_PI3);
	qy.set(Quat::Rot1::Y, MATH_7PI8);
	qz.set(Quat::Rot1::Z, MATH_PI2);

	qxyz = qx * qy * qz;
	qtmp.set(mxyz);

	QTEST(qxyz, qtmp);

	mtmp.set(qxyz);
	CHECK(eq_float(mtmp[m0], mxyz[m0]));
	CHECK(eq_float(mtmp[m1], mxyz[m1]));
	CHECK(eq_float(mtmp[m2], mxyz[m2]));
	CHECK(eq_float(mtmp[m3], mxyz[m3]));
	CHECK(eq_float(mtmp[m4], mxyz[m4]));
	CHECK(eq_float(mtmp[m5], mxyz[m5]));
	CHECK(eq_float(mtmp[m6], mxyz[m6]));
	CHECK(eq_float(mtmp[m7], mxyz[m7]));
	CHECK(eq_float(mtmp[m8], mxyz[m8]));
	CHECK(eq_float(mtmp[m9], mxyz[m9]));
	CHECK(eq_float(mtmp[m10], mxyz[m10]));
	CHECK(eq_float(mtmp[m11], mxyz[m11]));
	CHECK(eq_float(mtmp[m12], mxyz[m12]));
	CHECK(eq_float(mtmp[m13], mxyz[m13]));
	CHECK(eq_float(mtmp[m14], mxyz[m14]));
	CHECK(eq_float(mtmp[m15], mxyz[m15]));

	q1 = mx * my * qz;

	QTEST(q1, qxyz);

	Quat qyz;
	qyz = qy * qz;
	q1 = mx * qyz;

	QTEST(q1, qxyz);

	q1 = mx * (qy * qz);

	QTEST(q1, qxyz);

	q1 = mx * (my * qz);

	QTEST(q1, qxyz);

#endif
} TEST_END

// --- End of File ---