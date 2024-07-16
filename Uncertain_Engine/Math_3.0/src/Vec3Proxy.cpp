//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{
	// Add Proxy here
	Vec3Proxy::Vec3Proxy(const float _x, const float _y, const float _z)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	bool Vec3Proxy::operator==(const Vec3Proxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z) ==
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z);
	}

	bool Vec3Proxy::operator!=(const Vec3Proxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z) !=
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z);
	}

	bool Vec3Proxy::operator<(const Vec3Proxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z) <
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z);
	}

	bool Vec3Proxy::operator<=(const Vec3Proxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z) <=
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z);
	}

	bool Vec3Proxy::operator>(const Vec3Proxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z) >
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z);
	}

	bool Vec3Proxy::operator>=(const Vec3Proxy& mP) const
	{
		return
			(this->x * this->x + this->y * this->y + this->z * this->z) >=
			(mP.x * mP.x + mP.y * mP.y + mP.z * mP.z);
	}

	Vec3Proxy::operator float() const
	{
		return float(Trig::sqrt(((this->x * this->x) + (this->y * this->y) + (this->z * this->z))));
	}


}

//--- End of File ---
