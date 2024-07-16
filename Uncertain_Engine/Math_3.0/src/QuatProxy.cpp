//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{
	// Add Proxy here
		// Add Proxy here
	QuatProxy::QuatProxy(const float _x, const float _y, const float _z, const float _w)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
		this->w = _w;
	}

	bool QuatProxy::operator==(const QuatProxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w) ==
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z + mP.w * mP.w);
	}

	bool QuatProxy::operator!=(const QuatProxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w) !=
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z + mP.w * mP.w);
	}

	bool QuatProxy::operator<(const QuatProxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w) <
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z + mP.w * mP.w);
	}

	bool QuatProxy::operator<=(const QuatProxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w) <=
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z + mP.w * mP.w);
	}

	bool QuatProxy::operator>(const QuatProxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w) >
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z + mP.w * mP.w);
	}

	bool QuatProxy::operator>=(const QuatProxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w) >=
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z + mP.w * mP.w);
	}

	QuatProxy::operator float() const
	{
		return float(Trig::sqrt(((this->x * this->x) + (this->y * this->y) + (this->z * this->z + this->w * this->w))));
	}
}

//--- End of File ---
