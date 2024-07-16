//--------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "_Timer.h"

namespace Uncertain
{
	//---------------------------------------------------------------------------
	// CONSTRUCTORS / DESTRUCTORS / ASSIGNMENT:
	//---------------------------------------------------------------------------
	_Timer::_Timer() :
		privTicMark(_Time::Duration::MAX)
	{ }


	_Timer::~_Timer()
	{ }


	//---------------------------------------------------------------------------
	// TIMING METHODS:
	//---------------------------------------------------------------------------
	void _Timer::Tic()
	{
		this->privTicMark = _Timer::privGetSystemTime();
	}


	const _Time _Timer::Toc() const
	{
		_Time elapsedTime;

		// If tick has been called...
		if (_Time(_Time::Duration::MAX) != this->privTicMark)
		{
			elapsedTime = _Timer::privGetSystemTime() - this->privTicMark;
		}

		return(elapsedTime);
	}
}

// ---  End of File ---
