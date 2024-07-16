//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

//---------------------------------------------------------------------------
// HEADER FILES:
//---------------------------------------------------------------------------

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"

using namespace Uncertain;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST(Mat4_SET_MatrixRotInverseOrient, TestConfig::ALL)
{
#if Mat4_SET_MatrixRotInverseOrient

	// Orientation Type Constructor:
	Vec3  v17( 2.0f, 53.0f, 24.0f);
	Vec3  v18( 0.0f, -24.0f, 53.0f);
	Mat4 m57;
	m57.set(Mat4::Orient::WorldToLocal, v17, v18 );

	CHECK( eq_float(m57[m0],-0.9994f) );
	CHECK( eq_float(m57[m1], 0.0000f) );
	CHECK( eq_float(m57[m2], 0.0344f) );
	CHECK( (m57[m3] == 0.0f) );
	CHECK( eq_float(m57[m4], 0.0313f) );
	CHECK( eq_float(m57[m5],-0.4125f) );
	CHECK( eq_float(m57[m6], 0.9104f) );
	CHECK( (m57[m7] == 0.0f) );
	CHECK( eq_float(m57[m8],  0.0142f) );
	CHECK( eq_float(m57[m9],  0.9110f) );
	CHECK( eq_float(m57[m10], 0.4123f) );
	CHECK( (m57[m11] == 0.0f) );
	CHECK( (m57[m12] == 0.0f) );
	CHECK( (m57[m13] == 0.0f) );
	CHECK( (m57[m14] == 0.0f) );
	CHECK( eq_float(m57[m15],1.0f) );
	

#endif
} TEST_END

// ---  End of File ---------------
