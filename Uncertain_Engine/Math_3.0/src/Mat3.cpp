//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{
	// Do your magic here
	Mat3 Mat3::IDENTITY_MAT3(
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f);

	Mat3 Mat3::ZERO_MAT3(
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f);

	Mat3::Mat3()
	{
		_mm_store_ps(&this->_v0._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v1._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v2._vx, _mm_setzero_ps());
	}

	Mat3::Mat3(Vec3& a, Vec3& b, Vec3& c)
	{
		_mm_store_ps(&this->_v0._vx, a._mv);
		_mm_store_ps(&this->_v1._vx, b._mv);
		_mm_store_ps(&this->_v2._vx, c._mv);
	}

	Mat3::Mat3(float m0, float m1, float m2,
		float m4, float m5, float m6,
		float m8, float m9, float m10)
		: _m0(m0), _m1(m1), _m2(m2), _m3(0.0f),
		_m4(m4), _m5(m5), _m6(m6), _m7(0.0f),
		_m8(m8), _m9(m9), _m10(m10), _m11(0.0f),
		_m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
	}

	Mat3::Mat3(const Mat4& m)
	{
		_mm_store_ps(&this->_v0._vx, m._v0._mv);
		_mm_store_ps(&this->_v1._vx, m._v1._mv);
		_mm_store_ps(&this->_v2._vx, m._v2._mv);
	}

	Mat3& Mat3::operator=(Mat4 m)
	{
		_mm_store_ps(&this->_v0._vx, m._v0._mv);
		_mm_store_ps(&this->_v1._vx, m._v1._mv);
		_mm_store_ps(&this->_v2._vx, m._v2._mv);
		return *this;
	}

	Mat3::Mat3(Mat3::Special type)
	{
		switch (type)
		{
		case Mat3::Special::Identity:
			*this = IDENTITY_MAT3;
			break;
		case Mat3::Special::Zero:
			*this = ZERO_MAT3;
			break;
		}
	}

	Mat3::Mat3(const Mat3& m)
	{
		_mm_store_ps(&this->_v0._vx, m._v0._mv);
		_mm_store_ps(&this->_v1._vx, m._v1._mv);
		_mm_store_ps(&this->_v2._vx, m._v2._mv);
	}

	Mat3& Mat3::operator =(Mat3 m)
	{
		_mm_store_ps(&this->_v0._vx, m._v0._mv);
		_mm_store_ps(&this->_v1._vx, m._v1._mv);
		_mm_store_ps(&this->_v2._vx, m._v2._mv);
		return *this;
	}

	Mat3& Mat3::set(Mat3::Special type)
	{
		switch (type)
		{
		case Mat3::Special::Identity:
			*this = Mat3::IDENTITY_MAT3;
			break;
		case Mat3::Special::Zero:
			*this = Mat3::ZERO_MAT3;
			break;
		}

		return *this;
	}

	Mat3& Mat3::set(Mat3::Row row, const Vec3& v)
	{
		switch (row)
		{
		case Mat3::Row::i0:
			_mm_store_ps(&this->_v0._vx, v._mv);
			break;
		case Mat3::Row::i1:
			_mm_store_ps(&this->_v1._vx, v._mv);
			break;
		case Mat3::Row::i2:
			_mm_store_ps(&this->_v2._vx, v._mv);
			break;
		default:
			assert(false);
			break;
		}

		return *this;
	}

	Mat3& Mat3::set(const Vec3& a, const Vec3& b, const Vec3& c)
	{
		_mm_store_ps(&this->_v0._vx, a._mv);
		_mm_store_ps(&this->_v1._vx, b._mv);
		_mm_store_ps(&this->_v2._vx, c._mv);

		return *this;
	}

	void Mat3::clear()
	{
		_mm_store_ps(&this->_v0._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v1._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v2._vx, _mm_setzero_ps());
	}

	Vec3 Mat3::get(Mat3::Row row)
	{

		switch (row)
		{
		case Mat3::Row::i0:
			return Vec3(this->_v0);
			break;
		case Mat3::Row::i1:
			return Vec3(this->_v1);
			break;
		case Mat3::Row::i2:
			return Vec3(this->_v2);
			break;
		default:
			assert(false);
			return Vec3(this->_v0);
			break;
		}
	}

	const float Mat3::det() const
	{
		return ((this->_m0 * ((this->_m5 * this->_m10) - (this->_m9 * this->_m6)))
			- (this->_m1 * ((this->_m4 * this->_m10) - (this->_m6 * this->_m8)))
			+ (this->_m2 * ((this->_m4 * this->_m9) - (this->_m5 * this->_m8))));
	}

	Mat3& Mat3::adj()
	{
		*this = {
			((this->_m5 * this->_m10) - (this->_m6 * this->_m9)), ((this->_m2 * this->_m9) - (this->_m1 * this->_m10)), ((this->_m1 * this->_m6) - (this->_m2 * this->_m5)),
			((this->_m6 * this->_m8) - (this->_m4 * this->_m10)), ((this->_m0 * this->_m10) - (this->_m2 * this->_m8)), ((this->_m2 * this->_m4) - (this->_m0 * this->_m6)),
			((this->_m4 * this->_m9) - (this->_m5 * this->_m8)), ((this->_m1 * this->_m8) - (this->_m0 * this->_m9)), ((this->_m0 * this->_m5) - (this->_m1 * this->_m4))
		};

		return *this;
	}

	Mat3 Mat3::GetAdj() const
	{
		return Mat3(
			((this->_m5 * this->_m10) - (this->_m6 * this->_m9)), ((this->_m2 * this->_m9) - (this->_m1 * this->_m10)), ((this->_m1 * this->_m6) - (this->_m2 * this->_m5)),
			((this->_m6 * this->_m8) - (this->_m4 * this->_m10)), ((this->_m0 * this->_m10) - (this->_m2 * this->_m8)), ((this->_m2 * this->_m4) - (this->_m0 * this->_m6)),
			((this->_m4 * this->_m9) - (this->_m5 * this->_m8)), ((this->_m1 * this->_m8) - (this->_m0 * this->_m9)), ((this->_m0 * this->_m5) - (this->_m1 * this->_m4))
		);
	}

	Mat3& Mat3::T()
	{
		float a = this->_m4;
		this->_m4 = this->_m1;
		this->_m1 = a;

		a = this->_m8;
		this->_m8 = this->_m2;
		this->_m2 = a;

		a = this->_m9;
		this->_m9 = this->_m6;
		this->_m6 = a;

		return *this;
	}

	Mat3 Mat3::getT() const
	{
		Mat3 mRet = *this;

		float a = this->_m4;
		mRet._m4 = this->_m1;
		mRet._m1 = a;

		a = mRet._m8;
		mRet._m8 = mRet._m2;
		mRet._m2 = a;

		a = mRet._m9;
		mRet._m9 = mRet._m6;
		mRet._m6 = a;

		return mRet;
	}

	Mat3& Mat3::inv()
	{
		float det = this->det();
		if (fabs(det) >= MATH_TOLERANCE)
		{
			this->adj() /= det;
		}

		return *this;
	}

	Mat3 Mat3::getInv() const
	{
		Mat3 mRet(*this);
		float det = this->det();
		if (fabs(det) >= MATH_TOLERANCE)
		{
			mRet.adj() /= det;
		}
		return mRet;
	}

	bool Mat3::isIdentity(const float tolerance) const
	{
		// included in Vec4 operator ==
		AZUL_UNUSED_VAR(tolerance);

		return *this == Mat3::IDENTITY_MAT3;
	}

	bool Mat3::isIdentity() const
	{
		return *this == Mat3::IDENTITY_MAT3;
	}

	bool Mat3::isZero(const float tolerance) const
	{
		// included in Vec4 operator ==
		AZUL_UNUSED_VAR(tolerance);

		return *this == Mat3::ZERO_MAT3;
	}

	bool Mat3::isEqual(const Mat3& m)
	{
		return *this == m;
	}

	bool Mat3::isRotation() const
	{
		Mat3 m = *this * this->getT();

		return m.isIdentity();
	}


	const Mat3& Mat3::getIdentity()
	{
		return Mat3::IDENTITY_MAT3;
	}

	const Mat3& Mat3::getZero()
	{
		return Mat3::ZERO_MAT3;
	}

	Mat3 Mat3::operator+(const Mat3& m) const
	{
		Mat3 mRet;
		mRet._v0._mv = _mm_add_ps(this->_v0._mv, m._v0._mv);
		mRet._v1._mv = _mm_add_ps(this->_v1._mv, m._v1._mv);
		mRet._v2._mv = _mm_add_ps(this->_v2._mv, m._v2._mv);

		return mRet;
	}

	Mat3 Mat3::operator-(const Mat3& m) const
	{
		Mat3 mRet;
		mRet._v0._mv = _mm_sub_ps(this->_v0._mv, m._v0._mv);
		mRet._v1._mv = _mm_sub_ps(this->_v1._mv, m._v1._mv);
		mRet._v2._mv = _mm_sub_ps(this->_v2._mv, m._v2._mv);

		return mRet;
	}

	Mat3 Mat3::operator*(const Mat3& m) const
	{
		return Mat3(
			//0																	//1															//2
			(this->_m0 * m._m0 + this->_m1 * m._m4 + this->_m2 * m._m8), (this->_m0 * m._m1 + this->_m1 * m._m5 + this->_m2 * m._m9), (this->_m0 * m._m2 + this->_m1 * m._m6 + this->_m2 * m._m10),
			//4																	//5															//6
			(this->_m4 * m._m0 + this->_m5 * m._m4 + this->_m6 * m._m8), (this->_m4 * m._m1 + this->_m5 * m._m5 + this->_m6 * m._m9), (this->_m4 * m._m2 + this->_m5 * m._m6 + this->_m6 * m._m10),
			//8																	//9															//10
			(this->_m8 * m._m0 + this->_m9 * m._m4 + this->_m10 * m._m8), (this->_m8 * m._m1 + this->_m9 * m._m5 + this->_m10 * m._m9), (this->_m8 * m._m2 + this->_m9 * m._m6 + this->_m10 * m._m10)
		);
	}

	Mat3& Mat3::operator+=(const Mat3& m)
	{
		this->_v0._mv = _mm_add_ps(this->_v0._mv, m._v0._mv);
		this->_v1._mv = _mm_add_ps(this->_v1._mv, m._v1._mv);
		this->_v2._mv = _mm_add_ps(this->_v2._mv, m._v2._mv);

		return *this;
	}

	Mat3& Mat3::operator-=(const Mat3& m)
	{

		this->_v0._mv = _mm_sub_ps(this->_v0._mv, m._v0._mv);
		this->_v1._mv = _mm_sub_ps(this->_v1._mv, m._v1._mv);
		this->_v2._mv = _mm_sub_ps(this->_v2._mv, m._v2._mv);

		return *this;
	}

	Mat3& Mat3::operator/=(float rhs)
	{
		Vec4 inverse;
		inverse._mv = _mm_set1_ps(1.0f / rhs);

		this->_v0._mv = _mm_mul_ps(this->_v0._mv, inverse._mv);
		this->_v1._mv = _mm_mul_ps(this->_v1._mv, inverse._mv);
		this->_v2._mv = _mm_mul_ps(this->_v2._mv, inverse._mv);

		return *this;
	}

	Mat3& Mat3::operator*=(float rhs)
	{
		Vec4 fPacked;
		fPacked._mv = _mm_set1_ps(rhs);

		this->_v0._mv = _mm_mul_ps(this->_v0._mv, fPacked._mv);
		this->_v1._mv = _mm_mul_ps(this->_v1._mv, fPacked._mv);
		this->_v2._mv = _mm_mul_ps(this->_v2._mv, fPacked._mv);

		return *this;
	}

	Mat3 operator*(float f, const Mat3& m)
	{
		Mat3 mRet;

		Vec4 fPacked;
		fPacked._mv = _mm_set1_ps(f);

		mRet._v0._mv = _mm_mul_ps(fPacked._mv, m._v0._mv);
		mRet._v1._mv = _mm_mul_ps(fPacked._mv, m._v1._mv);
		mRet._v2._mv = _mm_mul_ps(fPacked._mv, m._v2._mv);

		return mRet;
	}

	Mat3 operator*(const Mat3& m, float f)
	{
		Mat3 mRet;
		Vec4 fPacked;

		fPacked._mv = _mm_set1_ps(f);

		mRet._v0._mv = _mm_mul_ps(m._v0._mv, fPacked._mv);
		mRet._v1._mv = _mm_mul_ps(m._v1._mv, fPacked._mv);
		mRet._v2._mv = _mm_mul_ps(m._v2._mv, fPacked._mv);

		return mRet;
	}

	Mat3& Mat3::operator*=(const Mat3& m)
	{

		Mat3 pTemp(
			//0																	//1															//2
			(this->_m0 * m._m0 + this->_m1 * m._m4 + this->_m2 * m._m8), (this->_m0 * m._m1 + this->_m1 * m._m5 + this->_m2 * m._m9), (this->_m0 * m._m2 + this->_m1 * m._m6 + this->_m2 * m._m10),
			//4																	//5															//6
			(this->_m4 * m._m0 + this->_m5 * m._m4 + this->_m6 * m._m8), (this->_m4 * m._m1 + this->_m5 * m._m5 + this->_m6 * m._m9), (this->_m4 * m._m2 + this->_m5 * m._m6 + this->_m6 * m._m10),
			//8																	//9															//10
			(this->_m8 * m._m0 + this->_m9 * m._m4 + this->_m10 * m._m8), (this->_m8 * m._m1 + this->_m9 * m._m5 + this->_m10 * m._m9), (this->_m8 * m._m2 + this->_m9 * m._m6 + this->_m10 * m._m10)
		);

		this->_v0 = pTemp._v0;
		this->_v1 = pTemp._v1;
		this->_v2 = pTemp._v2;

		return *this;
	}

	bool Mat3::operator ==(const Mat3& m) const
	{
		return (this->_v0 == Vec3(m._v0) &&
			this->_v1 == Vec3(m._v1) &&
			this->_v2 == Vec3(m._v2)
			);
	}

	Mat3 Mat3::operator+() const
	{
		return Mat3(*this);
	}

	Mat3 Mat3::operator-() const
	{
		Mat3 m;

		Vec4 v;
		_mm_store_ps(&v._vx, _mm_setzero_ps());
		m._v0._mv = _mm_sub_ps(v._mv, this->_v0._mv);
		m._v1._mv = _mm_sub_ps(v._mv, this->_v1._mv);
		m._v2._mv = _mm_sub_ps(v._mv, this->_v2._mv);

		return m;
	}


	void Mat3::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	};

}

// ---  End of File ---
