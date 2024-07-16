//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "_UnitTestConfiguration.h"
#include "MathEngine.h"

using namespace Uncertain;

//---------------------------------------------------------------------------
// TESTS:
//---------------------------------------------------------------------------

TEST( Quat_IsZero, TestConfig::ALL)
{
#if Quat_IsZero

	Quat  q1,q2;
	bool bvalue1;

	// isZero()
	q1.set(0.023f, 0.0f, 0.0f, 1.05f);
	
	// q1.isZero(0.0f) 
	bvalue1 = q1.isZero(0.0f);
	
	CHECK( bvalue1 == false );

#endif
} TEST_END

// ---  End of File ---------------
