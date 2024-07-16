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

TEST(Vec3_IsZeroFalse_default, TestConfig::ALL)
{
#if Vec3_IsZeroFalse_default

	bool bvalue1;
	Vec3 v1(0.023f, 0.0f, 0.0f);

	bvalue1 = v1.isZero();

	CHECK(bvalue1 == false);

#endif
} TEST_END


// ---  End of File ---------------
