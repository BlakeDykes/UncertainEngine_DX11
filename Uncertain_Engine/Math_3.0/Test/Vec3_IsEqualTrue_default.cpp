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

TEST(Vec3_IsEqualTrue_default, TestConfig::ALL)
{
#if Vec3_IsEqualTrue_default

	bool bvalue1;
	Vec3 v1(1.0f, 2.0f, 3.0f);
	Vec3 v2(1.0f, 2.0f, 3.05f);

	bvalue1 = v1.isEqual(v2);

	CHECK(bvalue1 == false);

#endif
} TEST_END

// ---  End of File ---------------
