//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"

using namespace Uncertain;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Quat_vectTimesQuat2, TestConfig::ALL)
{
#if Quat_vectTimesQuat2

	Quat	q1;
	Mat4	m1;
	Vec3	v1(1.0f, 2.0f, 3.0f);
	Vec4	v2, v3;

	q1.set(Quat::Rot::AxisAngle, Vec3(3, -4, 7), -MATH_2PI3);
	m1.set(Mat4::Rot::AxisAngle, Vec3(3, -4, 7), -MATH_2PI3);

	v2 = v1;
	v3 = v1;

	v2 = v1 * m1;
	v3 = v1 * q1;

	CHECK(eq_float(v2[x], v3[x]));
	CHECK(eq_float(v2[y], v3[y]));
	CHECK(eq_float(v2[z], v3[z]));
	CHECK(eq_float(v2[w], v3[w]));

#endif
} TEST_END

// --- End of File ---
