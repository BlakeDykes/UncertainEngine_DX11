//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{

	Vec4::Vec4()
	{
		_mm_store_ps(&this->_vx, _mm_setzero_ps());
	}

	Vec4::Vec4(float x, float y, float z, float w)
	{
		this->_mv = _mm_set_ps(w, z, y, x);
	}

	Vec4::Vec4(const Vec3& v, const float w)
	{
		_mm_store_ps(&this->_vx, v._mv);
		this->_vw = w;
	}


	Vec4::Vec4(const Vec4& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
	}

	Vec4& Vec4::operator =(const Vec4& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
		return *this;
	}


	void Vec4::set(const Vec3& v, const float w)
	{
		_mm_store_ps(&this->_vx, v._mv);
		this->_vw = w;
	}

	void Vec4::set(std::complex<float> c)
	{
		this->_mv = _mm_set_ps(c.real(), c.imag(), c.real(), c.imag());
	}

	Vec4& Vec4::set(const float x, const float y, const float z, const float w)
	{
		this->_mv = _mm_set_ps(w, z, y, x);

		return *this;
	}

	Vec4& Vec4::set(const Vec4& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
		return *this;
	}

	void Vec4::clear()
	{
		_mm_store_ps(&this->_vx, _mm_setzero_ps());
	}

	Vec4 Vec4::operator+(const Vec4& v) const
	{
		Vec4 a;
		a._mv = _mm_add_ps(this->_mv, v._mv);

		return a;
	}

	Vec4& Vec4::operator+=(const Vec4& v)
	{
		this->_mv = _mm_add_ps(this->_mv, v._mv);

		return *this;
	}

	Vec4 Vec4::operator-(const Vec4& v) const
	{
		Vec4 a;
		a._mv = _mm_sub_ps(this->_mv, v._mv);

		return a;
	}

	Vec4& Vec4::operator-=(const Vec4& v)
	{
		this->_mv = _mm_sub_ps(this->_mv, v._mv);

		return *this;
	}

	Vec4 Vec4::operator*(const Vec4& v) const
	{
		Vec4 a;
		a._mv = _mm_mul_ps(this->_mv, v._mv);

		return a;
	}

	Vec4& Vec4::operator*=(const Vec4& v)
	{
		this->_mv = _mm_mul_ps(this->_mv, v._mv);

		return *this;
	}

	Vec4 Vec4::operator/(const Vec4& v) const
	{
		Vec4 a;
		a._mv = _mm_div_ps(this->_mv, v._mv);

		return a;
	}

	Vec4& Vec4::operator/=(const Vec4& v)
	{
		this->_mv = _mm_div_ps(this->_mv, v._mv);

		return *this;
	}

	bool Vec4::operator==(const Vec4& v) const
	{

		////SIMD try
		////
		//// Set epsiollon
		//Vec4 eps;
		//eps._mv = _mm_set1_ps(MATH_TOLERANCE);

		//Vec4 absoluteDiff;
		//// Subtract this Vec4 with v, then AND with 0.0f
		//// andnot_ps - bitwise NOT of a, then bitwise AND with b
		//absoluteDiff._mv = _mm_andnot_ps(_mm_set1_ps(-0.0f), _mm_sub_ps(this->_mv, v._mv));

		//// returns true if none of the differences are less than MATH_TOLERANCE
		//// movemask_ps sets each bit based on most significant bit in argument
		//// cmplt_ps compares a and b, storing the less-than value in return
		//Vec4 returnMask;
		//returnMask._mv = _mm_cmplt_ps(absoluteDiff._mv, eps._mv);
		//int retBool = _mm_movemask_ps(_mm_cmplt_ps(absoluteDiff._mv, eps._mv));

		//return _mm_movemask_ps(_mm_cmplt_ps(absoluteDiff._mv, eps._mv)) != 0xF;

		//using fabs
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);
		float wEqual = (float)fabs(this->_vw - v._vw);

		return xEqual <= MATH_TOLERANCE &&
			yEqual <= MATH_TOLERANCE &&
			zEqual <= MATH_TOLERANCE &&
			wEqual <= MATH_TOLERANCE;
	}

	bool Vec4::operator!=(const Vec4& v) const
	{
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);
		float wEqual = (float)fabs(this->_vw - v._vw);

		return xEqual >= MATH_TOLERANCE ||
			yEqual >= MATH_TOLERANCE ||
			zEqual >= MATH_TOLERANCE ||
			wEqual >= MATH_TOLERANCE;
	}

	bool Vec4::operator==(const Vec3& v) const
	{
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);

		return xEqual <= MATH_TOLERANCE &&
			yEqual <= MATH_TOLERANCE &&
			zEqual <= MATH_TOLERANCE;
	}

	Vec4 Vec4::operator+(const float f) const
	{
		Vec4 a;
		a._mv = _mm_add_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec4 Vec4::operator-(const float f) const
	{
		Vec4 a;
		a._mv = _mm_sub_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec4 Vec4::operator*(const float f) const
	{
		Vec4 a;
		a._mv = _mm_mul_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec4 Vec4::operator/(const float f) const
	{
		Vec4 a;
		a._mv = _mm_div_ps(this->_mv, _mm_set1_ps(f));

		return a;
	}

	Vec4 operator*(float f, const Vec4& v)
	{
		Vec4 a;
		a._mv = _mm_mul_ps(v._mv, _mm_set1_ps(f));

		return a;
	}

	Vec4 operator*(const Vec4& v, const Mat3& m)
	{
		return Vec4(
			((m._m0 * v._vx) + (m._m4 * v._vy) + (m._m8 * v._vz)),
			((m._m1 * v._vx) + (m._m5 * v._vy) + (m._m9 * v._vz)),
			((m._m2 * v._vx) + (m._m6 * v._vy) + (m._m10 * v._vz)),
			v._vw
		);
	}

	Vec4 operator*(const Vec3& v, const Mat4& m)
	{
		return Vec4(
			((m._m0 * v._vx) + (m._m4 * v._vy) + (m._m8 * v._vz) + m._m12),
			((m._m1 * v._vx) + (m._m5 * v._vy) + (m._m9 * v._vz) + m._m13),
			((m._m2 * v._vx) + (m._m6 * v._vy) + (m._m10 * v._vz) + m._m14),
			((m._m3 * v._vx) + (m._m7 * v._vy) + (m._m11 * v._vz) + m._m15)
		);
	}

	Vec4 Vec4::operator*(const Mat4& m) const
	{
		return Vec4(
			((m._m0 * this->_vx) + (m._m4 * this->_vy) + (m._m8 * this->_vz) + (m._m12 * this->_vw)),
			((m._m1 * this->_vx) + (m._m5 * this->_vy) + (m._m9 * this->_vz) + (m._m13 * this->_vw)),
			((m._m2 * this->_vx) + (m._m6 * this->_vy) + (m._m10 * this->_vz) + (m._m14 * this->_vw)),
			((m._m3 * this->_vx) + (m._m7 * this->_vy) + (m._m11 * this->_vz) + (m._m15 * this->_vw))
		);
	}

	Vec4& Vec4::operator*=(const Mat4& m)
	{

		__m128 temp1 = _mm_mul_ps(_mm_set1_ps(this->_vx), m._v0._mv);
		__m128 temp2 = _mm_mul_ps(_mm_set1_ps(this->_vy), m._v1._mv);
		__m128 temp3 = _mm_mul_ps(_mm_set1_ps(this->_vz), m._v2._mv);
		__m128 temp4 = _mm_mul_ps(_mm_set1_ps(this->_vw), m._v3._mv);

		this->_mv = _mm_add_ps(_mm_add_ps(temp1, temp2), _mm_add_ps(temp3, temp4));

		return *this;
	}

	Vec4& Vec4::operator*=(const Mat3& m)
	{

		this->set(
			((m._m0 * this->_vx) + (m._m4 * this->_vy) + (m._m8 * this->_vz)),
			((m._m1 * this->_vx) + (m._m5 * this->_vy) + (m._m9 * this->_vz)),
			((m._m2 * this->_vx) + (m._m6 * this->_vy) + (m._m10 * this->_vz)),
			this->_vw
		);

		return *this;
	}


	Vec4& Vec4::operator*=(const float f)
	{
		this->_mv = _mm_mul_ps(this->_mv, _mm_set1_ps(f));

		return *this;
	}

	Vec4 Vec4::operator*(const Quat& q)
	{
		Vec3 v(*this);
		Quat qN = q.getNorm();

		return Vec4(
			Vec3(2 * qN._qw * (v.cross(qN._qV3) + qN._qV3 * (qN._qw * qN._qw - v.dot(v)) + 2 * (v.dot(qN._qV3) * v))),
			this->_vw
		);
	}

	Vec4& Vec4::operator*=(const Quat& q)
	{
		Vec3 v(*this);
		this->set(
			Vec3(2 * q._qw * (v.cross(q._qV3) + q._qV3 * (q._qw * q._qw - v.dot(v)) + 2 * (v.dot(q._qV3) * v))),
			this->_vw
		);

		return *this;
	}

	Vec4& Vec4::operator=(const Quat& q)
	{
		_mm_store_ps(&this->_vx, q._mq);

		return *this;
	}
	
	Vec4& Vec4::operator=(const Vec3& v)
	{
		_mm_store_ps(&this->_vx, v._mv);
		this->_vw = 1.0f;

		return *this;
	}

	Vec4 Vec4::operator+() const
	{
		return Vec4(*this);
	}

	Vec4 Vec4::operator-() const
	{
		Vec4 v;
		v._mv = _mm_sub_ps(_mm_setzero_ps(), this->_mv);

		return v;
	}


	Vec4 Vec4::cross(const Vec4& v) const
	{
		return Vec4(
			(this->_vy * v._vz) - (this->_vz * v._vy),
			(this->_vz * v._vx) - (this->_vx * v._vz),
			(this->_vx * v._vy) - (this->_vy * v._vx),
			1.0f
		);
	}

	float Vec4::dot(const Vec4& v) const
	{
		return float(
			(this->_vx * v._vx) + (this->_vy * v._vy) + (this->_vz * v._vz) + (this->_vw * v._vw)
			);
	}

	void Vec4::norm()
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

	Vec4 Vec4::getNorm() const
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

	const Vec4Proxy Vec4::len() const
	{
		return Vec4Proxy(this->_vx, this->_vy, this->_vz, this->_vw);
	}

	void Vec4::DFT_2Point_Real(Vec4& vOut)
	{
		float ar0 = this->_r0;
		float ar1 = this->_r1;

		float br0 = vOut._r0;
		float br1 = vOut._r1;

		this->_mv = _mm_set_ps(ar0 + br0, 0.0f, ar1 + br1, 0.0f);
		vOut._mv = _mm_set_ps(ar0 - br0, 0.0f, ar1 - br1, 0.0f);
	}

	void Vec4::DFT_ComplexMult(const Vec4& v1, const Vec4& v2)
	{
		this->_mv = _mm_set_ps(
			(v1._r0 * v2._r0) - (v1._i0 * v2._i0),
			(v1._r0 * v2._i0) + (v1._i0 * v2._r0),
			(v1._r1 * v2._r1) - (v1._i1 * v2._i1),
			(v1._r1 * v2._i1) + (v1._i1 * v2._r1)
		);
	}

	Vec4& Vec4::negateXYZ()
	{
		this->_mv = _mm_set_ps(this->_vw, -this->_vz, -this->_vy, -this->_vx);
		return *this;
	}

	float Vec4::getAngle(const Vec4& v) const
	{
		float dot = this->dot(v);
		float len1 = this->len();
		float len2 = v.len();

		dot = dot / len1 / len2;

		return Trig::acos(dot);
	}

	bool Vec4::isEqual(const Vec4& v, const float tolerance) const
	{
		float xEqual = (float)fabs(this->_vx - v._vx);
		float yEqual = (float)fabs(this->_vy - v._vy);
		float zEqual = (float)fabs(this->_vz - v._vz);
		float wEqual = (float)fabs(this->_vw - v._vw);

		return xEqual <= tolerance &&
			yEqual <= tolerance &&
			zEqual <= tolerance &&
			wEqual <= tolerance;
	}

	bool Vec4::isZero(const float tolerance) const
	{
		return (this->_vx >= 0.0f && this->_vx <= (0.0f + tolerance)) &&
			(this->_vy >= 0.0f && this->_vy <= (0.0f + tolerance)) &&
			(this->_vz >= 0.0f && this->_vz <= (0.0f + tolerance)) &&
			(this->_vw >= 0.0f && this->_vw <= (0.0f + tolerance));
	}


	void Vec4::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}
}

//--- End of File ---
