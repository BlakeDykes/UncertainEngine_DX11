//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"

using namespace Uncertain;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST( Quat_IsZero2, TestConfig::ALL)
{
#if Quat_IsZero2

	Quat  q1,q2;
	bool bvalue1;

	// isZero()
	q1.set(0.0f, 0.0f, 0.0f, 0.05f);
	
	// q1.isZero(0.1f) 
	bvalue1 = q1.isZero(0.1f);
	CHECK( bvalue1 == true );

#endif
} TEST_END


// ---  End of File ---------------
