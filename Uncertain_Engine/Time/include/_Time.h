//--------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef ANIM_TIME_H
#define ANIM_TIME_H

//---------------------------------------------------------------------------
// CLASS DECLARATION:
//---------------------------------------------------------------------------

namespace Uncertain
{
	class _Time
	{
	public:

		enum class Duration
		{
			// For constructing a _Time of zero. 
			ZERO,

			// For constructing one NTSC 60 hz frame of _Time.
			NTSC_FRAME,

			// For constructing one NTSC 60 hz frame of _Time. 
			NTSC_30_FRAME,

			// 24 Hz frames per second
			FILM_24_FRAME,

			// For constructing one PAL 50 hz frame of _Time. 
			PAL_FRAME,

			// For constructing one microsecond of _Time. 
			ONE_MICROSECOND,

			// For constructing one millisecond of _Time. 
			ONE_MILLISECOND,

			// For constructing one second of _Time. 
			ONE_SECOND,

			// For constructing one minute of _Time. 
			ONE_MINUTE,

			// For constructing one hour of _Time. 
			ONE_HOUR,

			// For constructing the most _negative_ _Time that can be represented.
			MIN,

			// For constructing the most positive _Time that can be represented.
			MAX,

			// insure the enum is size int
			DWORD = 0x7FFFFFFF
		};

	public:

		//-----------------------------------------------------------------
		// Friends
		//-----------------------------------------------------------------

		/*
		* return: The product of the specified float and _Time.
		*
		* Example:
		*    const _Time halfSecond = 0.5f * _Time(ONE_SECOND);
		*/
		friend const _Time operator*(const float, const _Time &);

		/*
		* return: The product of the specified integer and _Time.
		*
		* Example:
		*    const _Time fourMinutes = 4 * _Time(ONE_MINUTE);
		*/
		friend const _Time operator*(const int, const _Time &);

		//-----------------------------------------------------------------
		// Constructors / Destructors / Assignment
		//-----------------------------------------------------------------

		// Creates a _Time equal to _Time(ZERO). 
		inline _Time();

		/*
		* Creates the specific amount of _Time.
		*
		* Example:
		*    const _Time oneSecond( ONE_SECOND );
		*/
		explicit _Time(const Duration);

		// Creates a copy of the specified _Time. 
		inline _Time(const _Time &);

		/*
		* Assigns the right-hand side to this _Time.
		* return: this _Time.
		*
		*  Example:
		*    const _Time tickDuration = _Time(NTSC_DURATION);
		*/
		inline _Time &operator=(const _Time &rhs);

		// destrutor
		~_Time()
		{ }

		//-----------------------------------------------------------------
		// Comparisons
		//-----------------------------------------------------------------

		/*
		* return: true if this _Time is equal to the right-hand side.
		*
		*  Example:
		*    assert( _Time(ONE_HOUR)   == 60 * _Time(ONE_MINUTE) );
		*    assert( _Time(ONE_MINUTE) == 60 * _Time(ONE_SECOND) );
		*/
		bool operator==(const _Time &rhs) const;

		/*
		* return: true if this _Time is not equal to the right-hand side.
		*/
		bool operator!=(const _Time &rhs) const;

		/*
		* return: true if this _Time is less than the right-hand side.
		*
		* Example:
		*    bool couldBeOttosMile_Time( const _Time t )
		*    {
		*       return( t < 4 * _Time(ONE_MINUTE) );
		*    }
		*/
		bool operator<(const _Time &rhs) const;

		/*
		* return: true if this _Time is less or equal to the right-hand side.
		*/
		bool operator<=(const _Time &rhs) const;

		/*
		* return: true if this _Time is greater than the right-hand side.
		*/
		bool operator>(const _Time &rhs) const;

		/*
		* return: true if this _Time is greather than or equal to
		*  the right-hand side.
		*/
		bool operator>=(const _Time &rhs) const;

		//-----------------------------------------------------------------
		// Negation / Addition / Subtraction
		//-----------------------------------------------------------------

		/*
		* return: A negative copy of this _Time.
		*
		* Example:
		*    void neverFail( const _Time t )
		*    {
		*       assert( t + (-t) == _Time(ZERO) );
		*    }
		*/
		const _Time operator-() const;

		/*
		* return: The sum of this _Time and the right-hand side.
		*
		* Example:
		*    void neverFail( const _Time t )
		*    {
		*       assert( t + t == 2 * t );
		*    }
		*/
		const _Time operator+(const _Time &rhs) const;

		/*
		* return: The difference between this _Time and the right-hand side.
		*
		* Example:
		*    void neverFail( const _Time t )
		*    {
		*       assert( t - t == _Time(ZERO) );
		*    }
		*/
		const _Time operator-(const _Time &rhs) const;

		/*
		* Adds the right-hand side to this _Time.
		*
		* return: This _Time.
		*
		*  Example:
		*    void zeroOut_Time( _Time& t )
		*    {
		*       t += (-t);
		*       assert( _Time(ZERO) == t );
		*    }
		*/
		_Time &operator+=(const _Time &rhs);

		/*
		* Subtracts the right-hand side from this _Time.
		*
		* return: This _Time.
		*
		*  Example:
		*    void zeroOut_Time( _Time& t )
		*    {
		*       t -= t;
		*       assert( _Time(ZERO) == t );
		*    }
		*/
		_Time &operator-=(const _Time &rhs);

		//-----------------------------------------------------------------
		// Multiplication
		//-----------------------------------------------------------------

		/*
		* return: The product of this _Time and the specified float.
		*
		* Example:
		*    const _Time halfSecond = _Time(ONE_SECOND) * 0.5f;
		*/
		const _Time operator*(const float) const;

		/*
		* return: The product of this _Time and the specified integer.
		*
		*  Example:
		*    const _Time fourMinutes = _Time(ONE_MINUTE) * 4;
		*/
		const _Time operator*(const int) const;

		/*
		* Multiplies this _Time by the specified float.
		*
		* return: This _Time.
		*
		* Example:
		*    void decay_Time( _Time& t )
		*    {
		*       t *= 0.95f;
		*    }
		*/
		_Time &operator*=(const float);

		/*
		* Multiplies this _Time by the specified integer.
		*
		* return: This _Time.
		*
		* Example:
		*    void double_Time( _Time& t )
		*    {
		*       t *= 2;
		*    }
		*/
		_Time &operator*=(const int);

		//-----------------------------------------------------------------
		// name: Division
		//-----------------------------------------------------------------

		/*
		* return: The ratio of this _Time divided by the denominator _Time.
		*
		* note: Dividing by _Time::ZERO is undefined.
		*/
		float operator/(const _Time &denominator) const;

		/*
		* return: The result of dividing this _Time and by the
		* specified float.
		*
		* note: Dividing by zero is undefined.
		*/
		const _Time operator/(const float denominator) const;

		/*
		* return: The result of dividing this _Time and by the
		* specified integer.
		*
		* note: Dividing by zero is undefined.
		*/
		const _Time operator/(const int denominator) const;

		/*
		* Divides this _Time by the specified float.
		*
		* return: This _Time.
		*
		* note: Dividing by zero is undefined.
		*/
		_Time &operator/=(const float);

		/*
		* Divides this _Time by the specified integer.
		*
		* return: This _Time.
		*
		* note: Dividing by zero is undefined.
		*/
		_Time &operator/=(const int);

		//-----------------------------------------------------------------
		// Quotient / Remainder
		//-----------------------------------------------------------------

		/*
		* return: The <i>truncated</i>, integral division of the
		* numerator and denominator.  In other words,
		* the result of the division is rounded down to the nearest
		* integer.
		*
		* note: Dividing by _Time(ZERO) is undefined.
		*/
		static int 	Quotient(const _Time &numerator, const _Time &denominator);

		/*
		* return: The remainder of dividing the numerator into the
		* denominator.  This is the equivalent of a modulus operation.
		*
		* note: Dividing by _Time(ZERO) is undefined.
		*/
		static const _Time 	Remainder(const _Time &numerator, const _Time &denominator);

	private:
		// types
		typedef long long Representation;

		// constructor
		explicit _Time(const Representation);

		// data
		Representation privRaw_Time;
	};


	//---------------------------------------------------------------------------
	// INLINED IMPLEMENTATION:
	//---------------------------------------------------------------------------
	_Time::_Time() : privRaw_Time(0)
	{
	}


	_Time::_Time(const _Time &rhs) : privRaw_Time(rhs.privRaw_Time)
	{
	}


	_Time &_Time::operator=(const _Time &rhs)
	{
		if (this != &rhs)
		{
			this->privRaw_Time = rhs.privRaw_Time;
		}

		return(*this);
	}


}

#endif  

// ---  End of File ---
