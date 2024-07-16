//--------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#include "_Time.h"

namespace Uncertain
{
	//---------------------------------------------------------------------------
	// FRIENDS:
	//---------------------------------------------------------------------------
	const _Time operator*(const float ratio, const _Time &rhs)
	{
		return(_Time(static_cast<_Time::Representation>((double)ratio * (double)rhs.privRaw_Time)));
	}

	const _Time operator*(const int ratio, const _Time &rhs)
	{
		return(_Time(ratio * rhs.privRaw_Time));
	}

	//---------------------------------------------------------------------------
	// CONSTRUCTORS:
	//---------------------------------------------------------------------------

	_Time::_Time(const Duration duration) : privRaw_Time(0)
	{
		// IMPORTANT: This is private information that is SUBJECT TO CHANGE!
		//
		// Currently: 1 second = 30000000
		//            1 us     = 30
		//            1 ms     = 30000
		//            1 NTSC   = 500000
		//            1 PAL    = 600000
		//
		// At 32 bits, this gives us a range of roughly -20 to 20 hours.
		const _Time::Representation ONE_RAW_SECOND = 30000000;

		switch (duration)
		{
		case Duration::ZERO:
			this->privRaw_Time = 0;
			break;

		case Duration::NTSC_FRAME:
			this->privRaw_Time = ONE_RAW_SECOND / 60;
			break;

		case Duration::FILM_24_FRAME:
			this->privRaw_Time = ONE_RAW_SECOND / 24;
			break;

		case Duration::NTSC_30_FRAME:
			this->privRaw_Time = 2 * ONE_RAW_SECOND / 60;
			break;

		case Duration::PAL_FRAME:
			this->privRaw_Time = ONE_RAW_SECOND / 50;
			break;

		case Duration::ONE_SECOND:
			this->privRaw_Time = ONE_RAW_SECOND;
			break;

		case Duration::ONE_MILLISECOND:
			this->privRaw_Time = ONE_RAW_SECOND / 1000;
			break;

		case Duration::ONE_MICROSECOND:
			this->privRaw_Time = ONE_RAW_SECOND / 1000000;
			break;

		case Duration::ONE_MINUTE:
			this->privRaw_Time = 60 * ONE_RAW_SECOND;
			break;

		case Duration::ONE_HOUR:
			this->privRaw_Time = 60 * 60 * ONE_RAW_SECOND;
			break;

		case Duration::MIN:
			this->privRaw_Time = std::numeric_limits< _Time::Representation >::min();
			break;

		case Duration::MAX:
			this->privRaw_Time = std::numeric_limits< _Time::Representation >::max();
			break;

		case Duration::DWORD:
			assert(false);
			this->privRaw_Time = 0;
			break;

		default:
			assert(false);
			this->privRaw_Time = 0;
			break;
		}
	}

	//---------------------------------------------------------------------------
	// COMPARISONS:
	//---------------------------------------------------------------------------
	bool _Time::operator==(const _Time &rhs) const
	{
		return(this->privRaw_Time == rhs.privRaw_Time);
	}

	bool _Time::operator!=(const _Time &rhs) const
	{
		return(this->privRaw_Time != rhs.privRaw_Time);
	}

	bool _Time::operator<(const _Time &rhs) const
	{
		return(this->privRaw_Time < rhs.privRaw_Time);
	}

	bool _Time::operator<=(const _Time &rhs) const
	{
		return(this->privRaw_Time <= rhs.privRaw_Time);
	}

	bool _Time::operator>(const _Time &rhs) const
	{
		return(this->privRaw_Time > rhs.privRaw_Time);
	}

	bool _Time::operator>=(const _Time &rhs) const
	{
		return(this->privRaw_Time >= rhs.privRaw_Time);
	}

	//---------------------------------------------------------------------------
	// NEGATION / ADDITION / SUBTRACTION:
	//---------------------------------------------------------------------------
	const _Time _Time::operator-() const
	{
		return(_Time(-this->privRaw_Time));
	}

	const _Time _Time::operator+(const _Time &rhs) const
	{
		// !!! FIXME: Overflow checks
		return(_Time(this->privRaw_Time + rhs.privRaw_Time));
	}

	const _Time _Time::operator-(const _Time &rhs) const
	{
		// !!! FIXME: Overflow checks
		return(_Time(this->privRaw_Time - rhs.privRaw_Time));
	}

	_Time &_Time::operator+=(const _Time &rhs)
	{
		// !!! FIXME: Overflow checks
		this->privRaw_Time += rhs.privRaw_Time;
		return(*this);
	}

	_Time &_Time::operator-=(const _Time &rhs)
	{
		// !!! FIXME: Overflow checks
		this->privRaw_Time -= rhs.privRaw_Time;
		return(*this);
	}

	//---------------------------------------------------------------------------
	// MULTIPLICATION:
	//---------------------------------------------------------------------------
	const _Time _Time::operator*(const float ratio) const
	{
		// !!! FIXME: Overflow checks
		return(_Time(static_cast<Representation>((double)ratio * (double)this->privRaw_Time)));
	}

	const _Time _Time::operator*(const int ratio) const
	{
		// !!! FIXME: Overflow checks
		return(_Time(this->privRaw_Time * ratio));
	}

	_Time &_Time::operator*=(const float ratio)
	{
		// !!! FIXME: Overflow checks
		this->privRaw_Time = static_cast<Representation>((double)ratio * (double)this->privRaw_Time);
		return(*this);
	}

	_Time &_Time::operator*=(const int ratio)
	{
		// !!! FIXME: Overflow checks
		this->privRaw_Time *= ratio;
		return(*this);
	}


	//---------------------------------------------------------------------------
	// DIVISION:
	//---------------------------------------------------------------------------
	float _Time::operator/(const _Time &denominator) const
	{
		// !!! FIXME: Divide by zero.
		return(static_cast<float>(this->privRaw_Time) / (float)denominator.privRaw_Time);
	}


	const _Time _Time::operator/(const float denominator) const
	{
		// !!! FIXME: Divide by zero.
		return(_Time(static_cast<Representation>((float)this->privRaw_Time / denominator)));
	}

	const _Time _Time::operator/(const int denominator) const
	{
		assert((float)denominator != 0.0f);
		return(_Time(this->privRaw_Time / denominator));
	}

	_Time &_Time::operator/=(const float denominator)
	{
		assert(denominator != 0.0f);
		this->privRaw_Time = (Representation)((double)this->privRaw_Time / (double)denominator);
		return(*this);
	}

	_Time &_Time::operator/=(const int denominator)
	{
		assert((float)denominator != 0.0f);
		this->privRaw_Time /= denominator;
		return(*this);
	}

	//---------------------------------------------------------------------------
	// DIVISION:
	//---------------------------------------------------------------------------
	int 	_Time::Quotient(const _Time &numerator, const _Time &denominator)
	{
		// !!! FIXME: Divide by zero & check range
		assert((float)denominator.privRaw_Time != 0.0f);
		return(static_cast<int> (numerator.privRaw_Time / denominator.privRaw_Time));
	}

	const _Time 	_Time::Remainder(const _Time &numerator, const _Time &denominator)
	{
		return(_Time(numerator.privRaw_Time % denominator.privRaw_Time));
	}

	//---------------------------------------------------------------------------
	// PRIVATE IMPLEMENTATION:
	//---------------------------------------------------------------------------
	_Time::_Time(const _Time::Representation raw_Time) : privRaw_Time(raw_Time)
	{
	}

}

// ---  End of File ---
