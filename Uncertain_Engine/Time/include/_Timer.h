//--------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef _TIMER_H
#define _TIMER_H

/*
 * class _Timer
 * Measures real durations of time in terms of Time.
 *
 * The _Timer interface you see here owes itself to Matlab.
 * You only measure segements of time with a _Timer.  Use tic() to
 * mark the beginning of the segment.  Use toc() to mark the end.
 *
 * toc() always returns the amount of time that has elapsed since tic()
 * was called.  If you never call tic() on your _Timer then toc() will
 * return @link Time::Time() zero @endlink.
 *
 *
 * Example
 *
 *    #include "_Timer.h"
 *
 *    // Create a _Timer.
 *    _Timer _Timer;
 *
 *    // Mark our begin time.
 *    _Timer.tic();
 *
 *    // Do something for a while.
 *    ...
 *
 *    // Measure how much time has gone by since we called tic().
 *    const Time elapsedTime = _Timer.toc();
 *
 *
 */

 //---------------------------------------------------------------------------
 // HEADER FILES:
 //---------------------------------------------------------------------------

#include "_Time.h"

//---------------------------------------------------------------------------
// TEMPLATE DECLARATION:
//---------------------------------------------------------------------------

namespace Uncertain
{
	class _Timer
	{
	public:
		//-----------------------------------------------------------------
		// Constructors / Destructors
		//-----------------------------------------------------------------

		/* Creates a _Timer for the platform you are compiling on. */
		_Timer();
		_Timer(const _Timer &) = delete;
		_Timer &operator = (const _Timer &) = delete;
		~_Timer();

		//-----------------------------------------------------------------
		// Timing Methods
		//-----------------------------------------------------------------

		/*
		* Marks the moment in time that toc() will get measured against.
		* Call tic() to mark the beginning of the segment of time that
		* you want to measure.
		*/
		void Tic();

		/*
		* return: The duration of Time that has elapsed since tic() was
		* called.  Returns Time::Time() zero if tic() has
		* never been called on this _Timer.
		*/
		const _Time Toc() const;


	private:
		// static method
		static const _Time privGetSystemTime();

		// Data
		_Time privTicMark;
	};
}

#endif   // _Timer_h

// ---  End of File ---
