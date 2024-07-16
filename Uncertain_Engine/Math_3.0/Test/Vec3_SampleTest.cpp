//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"
using namespace Uncertain;

//---------------------------------------------------------------------------
// Sample TESTS:
//---------------------------------------------------------------------------

TEST(Vec3_SampleTest, TestConfig::ALL)
{
#if Vec3_SampleTest

	const float x(5.0f);

	CHECK(x == 5.0f);

#endif
} TEST_END

// ---  End of File ---------------
