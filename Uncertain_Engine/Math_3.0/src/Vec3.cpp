//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{

	Vec3::Vec3()
	{
		_mm_store_ps(&this->_vx, _mm_setzero_ps());
	}

	Vec3::Vec3(float x, float y, float z)
	{
		this->_mv = _mm_set_ps(0.0f, z, y, x);
	}

	Vec3::Vec3(const Vec4& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
	}


	Vec3::Vec3(const Vec3& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
	}

	Vec3& Vec3::operator =(const Vec3& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
		return *this;
	}

	Vec3& Vec3::operator =(const Vec4& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
		return *this;
	}


	Vec3& Vec3::set(const float x, const float y, const float z)
	{
		this->_mv = _mm_set_ps(0.0f, z, y, x);

		return *this;
	}

	void Vec3::set(const Vec3& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
	}

	void Vec3::set(const Vec4& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
	}


	void Vec3::clear()
	{
		_mm_store_ps(&this->_vx, _mm_setzero_ps());
	}

	Vec3 Vec3::operator+(const Vec3& v) const
	{
		Vec3 a;
		a._mv = _mm_add_ps(this->_mv, v._mv);

		return a;
	}

	Vec3 Vec3::operator+=(const Vec3& v)
	{
		this->_mv = _mm_add_ps(this->_mv, v._mv);

		return *this;
	}

	Vec3 Vec3::operator-(const Vec3& v) const
	{
		Vec3 a;
		a._mv = _mm_sub_ps(this->_mv, v._mv);

		return a;
	}

	Vec3 Vec3::operator-=(const Vec3& v)
	{
		this->_mv = _mm_sub_ps(this->_mv, v._mv);

		return *this;
	}

	Vec3 Vec3::operator*(const Vec3& v) const
	{
		Vec3 a;
		a._mv = _mm_mul_ps(this->_mv, v._mv);

		return a;
	}

	Vec3 Vec3::operator*=(const Vec3& v)
	{
		this->_mv = _mm_mul_ps(this->_mv, v._mv);

		return *this;
	}

	Vec3 Vec3::operator/(const Vec3& v) const
	{
		Vec3 a;
		a._mv = _mm_div_ps(this->_mv, v._mv);

		return a;
	}

	Vec3 Vec3::operator/=(const Vec3& v)
	{
		this->_mv = _mm_div_ps(this->_mv, v._mv);

		return *this;
	}

	bool Vec3::operator==(const Vec3& v) const
	{
		//using fabs
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);

		return xEqual <= MATH_TOLERANCE &&
			yEqual <= MATH_TOLERANCE &&
			zEqual <= MATH_TOLERANCE;
	}

	bool Vec3::operator==(const Vec4& v) const
	{
		//using fabs
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);

		return xEqual <= MATH_TOLERANCE &&
			yEqual <= MATH_TOLERANCE &&
			zEqual <= MATH_TOLERANCE;
	}


	Vec3 Vec3::operator+(const float f) const
	{
		Vec3 a;
		a._mv = _mm_add_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec3 Vec3::operator-(const float f) const
	{
		Vec3 a;
		a._mv = _mm_sub_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec3 Vec3::operator*(const float f) const
	{
		Vec3 a;
		a._mv = _mm_mul_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec3 Vec3::operator/(const float f) const
	{
		Vec3 a;
		a._mv = _mm_div_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec3 operator*(float f, const Vec3& v)
	{
		Vec3 a;
		a._mv = _mm_mul_ps(v._mv, _mm_set1_ps(f));

		return a;
	}

	Vec3 Vec3::operator*(const Mat3& m) const
	{
		return Vec3(
			((m._m0 * this->_vx) + (m._m4 * this->_vy) + (m._m8 * this->_vz)),
			((m._m1 * this->_vx) + (m._m5 * this->_vy) + (m._m9 * this->_vz)),
			((m._m2 * this->_vx) + (m._m6 * this->_vy) + (m._m10 * this->_vz))
		);
	}

	Vec3& Vec3::operator*=(const Mat3& m)
	{

		this->set(
			((m._m0 * this->_vx) + (m._m4 * this->_vy) + (m._m8 * this->_vz)),
			((m._m1 * this->_vx) + (m._m5 * this->_vy) + (m._m9 * this->_vz)),
			((m._m2 * this->_vx) + (m._m6 * this->_vy) + (m._m10 * this->_vz))
		);

		return *this;
	}

	Vec3 Vec3::operator*(const Quat& q)
	{
		// Credit - https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
		//Vec3 t = 2 * q._qV3.cross(*this);
		//return *this + q._qw * t + q._qV3.cross(t);

		Vec3 out;
		q.Lqcvq(*this, out);
		return out;
	}

	Vec3& Vec3::operator*=(const Quat& q)
	{
		//Vec3 t = 2 * q._qV3.cross(*this);
		//*this = *this + q._qw * t + q._qV3.cross(t);

		Vec3 out;
		q.Lqcvq(*this, out);
		*this = out;

		return *this;
	}

	Vec3& Vec3::operator=(const Quat& q)
	{
		_mm_store_ps(&this->_vx, q._mq);

		return *this;
	}

	Vec3& Vec3::operator*=(const float f)
	{
		this->_mv = _mm_mul_ps(this->_mv, _mm_set1_ps(f));

		return *this;
	}

	Vec3 Vec3::operator+() const
	{
		Vec3 v;

		v._mv = _mm_set_ps(0.0f, this->_vz, this->_vy, this->_vx);

		return v;
	}

	Vec3 Vec3::operator-() const
	{
		Vec3 v;

		v._mv = _mm_set_ps(0.0f, -this->_vz, -this->_vy, -this->_vx);

		return v;
	}


	Vec3 Vec3::cross(const Vec3& v) const
	{
		return Vec3(
			(this->_vy * v._vz) - (this->_vz * v._vy),
			(this->_vz * v._vx) - (this->_vx * v._vz),
			(this->_vx * v._vy) - (this->_vy * v._vx)
		);
	}

	float Vec3::dot(const Vec3& v) const
	{
		return float(
			(this->_vx * v._vx) + (this->_vy * v._vy) + (this->_vz * v._vz)
			);
	}

	void Vec3::norm()
	{
		float len = this->len();

		if (len != 0.0f)
		{
			*this = *this / len;
		}
		else
		{
			assert(false);
		}
	}

	Vec3 Vec3::getNorm() const
	{
		float len = this->len();

		if (len != 0.0f)
		{
			return *this / len;
		}
		else
		{
			assert(false);
			return *this;
		}
	}

	const Vec3Proxy Vec3::len() const
	{
		return Vec3Proxy(this->_vx, this->_vy, this->_vz);
	}

	float Vec3::getAngle(const Vec3& v) const
	{
		float dot = this->dot(v);
		float len1 = this->len();
		float len2 = v.len();

		dot = dot / len1 / len2;

		return Trig::acos(dot);
	}

	bool Vec3::isEqual(const Vec3& v, const float tolerance) const
	{
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);

		return xEqual <= tolerance &&
			yEqual <= tolerance &&
			zEqual <= tolerance
			;
	}

	bool Vec3::isZero(const float tolerance) const
	{
		return (this->_vx >= 0.0f && this->_vx <= (0.0f + tolerance)) &&
			(this->_vy >= 0.0f && this->_vy <= (0.0f + tolerance)) &&
			(this->_vz >= 0.0f && this->_vz <= (0.0f + tolerance))
			;
	}


	void Vec3::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}
}

//--- End of File ---
