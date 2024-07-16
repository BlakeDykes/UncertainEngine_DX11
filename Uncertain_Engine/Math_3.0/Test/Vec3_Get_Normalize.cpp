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

TEST(Vec3_Get_Normalize, TestConfig::ALL)
{
#if Vec3_Get_Normalize

	Vec3 v1(1.0f, 2.0f, 3.0f);
	Vec3 v2;

	v2 = v1.getNorm();

	CHECK(eq_float(v2[x], 0.2672f));
	CHECK(eq_float(v2[y], 0.5345f));
	CHECK(eq_float(v2[z], .80174f));

	CHECK((v1[x] == 1.0f));
	CHECK((v1[y] == 2.0f));
	CHECK((v1[z] == 3.0f));

#endif
} TEST_END

// ---  End of File ---------------
