//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{ 

	Quat::Quat()
	{
		_mm_store_ps(&this->_qx, _mm_setzero_ps());
		this->_qw = 1.0f;
	}

	Quat::Quat(float x, float y, float z, float w)
	{
		this->_mq = _mm_set_ps(w, z, y, x);
	}

	Quat::Quat(const Quat& q)
	{
		_mm_store_ps(&this->_qx, q._mq);

	}

	Quat& Quat::operator=(const Quat& q)
	{
		_mm_store_ps(&this->_qx, q._mq);
		return *this;
	}

	Quat::Quat(const Vec4& v)
	{
		_mm_store_ps(&this->_qx, v._mv);
	}

	Quat::Quat(const Vec3& v, float w)
	{
		_mm_store_ps(&this->_qx, v._mv);
		this->_qw = w;
	}

	Quat::Quat(const Mat4& m)
	{
		float tr = m._m0 + m._m5 + m._m10;
		float s;

		if (tr > 0.0f)
		{
			s = 2 * sqrtf(tr + 1.0f);
			this->_qw = 0.25f * s;
			this->_qx = -(m._m9 - m._m6) / s;
			this->_qy = -(m._m2 - m._m8) / s;
			this->_qz = -(m._m4 - m._m1) / s;
		}
		else if ((m._m0 > m._m5) && (m._m0 > m._m10))
		{
			s = 2 * sqrtf(1.0f + m._m0 - m._m5 - m._m10);
			this->_qw = -(m._m9 - m._m6) / s;
			this->_qx = 0.25f * s;
			this->_qy = (m._m1 + m._m4) / s;
			this->_qz = (m._m2 + m._m8) / s;
		}
		else if (m._m5 > m._m10)
		{
			s = 2 * sqrtf(1.0f + m._m5 - m._m0 - m._m10);
			this->_qw = -(m._m2 - m._m8) / s;
			this->_qx = (m._m1 + m._m4) / s;
			this->_qy = 0.25f * s;
			this->_qz = (m._m6 + m._m9) / s;
		}
		else
		{
			s = 2 * sqrtf(1.0f + m._m10 - m._m0 - m._m5);
			this->_qw = -(m._m4 - m._m1) / s;
			this->_qx = (m._m2 + m._m8) / s;
			this->_qy = (m._m6 + m._m9) / s;
			this->_qz = 0.25f * s;
		}

	}

	Quat::Quat(Quat::Rot1 rot, float angle)
	{
		_mm_store_ps(&this->_qx, _mm_setzero_ps());
		float halfAngle = angle * 0.5f;
		this->_qw = Trig::cos(halfAngle);

		switch(rot)
		{
			case Quat::Rot1::X:
			{
				this->_qx = Trig::sin(halfAngle);
				break;
			}
			case Quat::Rot1::Y:
			{
				this->_qy = Trig::sin(halfAngle);
				break;
			}
			case Quat::Rot1::Z:
			{
				this->_qz = Trig::sin(halfAngle);
				break;
			}
			default:
				assert(false);
				break;
		}
	}

	Quat::Quat(const Quat::Rot, const Vec3& axis, const float angle)
	{
		const float a = 0.5f * angle;
		float cos_a = cosf(a);
		float sin_a = sinf(a);

		Vec3 v = axis.getNorm();

		v *= sin_a;
		this->_qx = v[x];
		this->_qy = v[y];
		this->_qz = v[z];
		this->_qw = cos_a;
	}

	Quat::Quat(const Quat::Rot3, const float x, const float y, const float z)
	{
		Quat qx(Quat::Rot1::X, x);
		Quat qy(Quat::Rot1::Y, y);
		Quat qz(Quat::Rot1::Z, z);

		*this = (qx * qy * qz).norm();
	}

	Quat::Quat(const Quat::Special type)
	{
		switch (type)
		{
			case Quat::Special::Zero: 
			{
				_mm_store_ps(&this->_qx, _mm_setzero_ps());
				break;
			}
			case Quat::Special::Identity:
			{
				_mm_store_ps(&this->_qx, _mm_setzero_ps());
				this->_qw = 1.0f;
				break;
			}
			default:
				assert(false);
		}
	}

	Quat::Quat(const Quat::Orient type, const Vec3& dof, const Vec3& up)
	{
		//Vec3 z = dof;
		//Vec3 x = up.cross(z);
		//Vec3 y = z.cross(x);

		//x.norm();
		//y.norm();
		//z.norm();

		switch (type)
		{
			case Quat::Orient::LocalToWorld: {
				*this = Quat(Mat4(Mat4::Orient::LocalToWorld, dof, up));
				break;
			}
			case Quat::Orient::WorldToLocal: {
				*this = Quat(Mat4(Mat4::Orient::WorldToLocal, dof, up));
				break;
			}
			default:
				assert(false);
		}
	}

	Quat& Quat::norm()
	{
		float mag = this->magSquared();
		if (mag > MATH_TOLERANCE)
		{
			mag = 1.0f / sqrtf(mag);
			this->_mq = _mm_mul_ps(this->_mq, _mm_set1_ps(mag));
		}

		return *this;
	}

	Quat Quat::getNorm() const
	{
		Quat q = *this;
		float mag = q.magSquared();
		if (mag > MATH_TOLERANCE)
		{
			mag = 1.0f / sqrtf(mag);
			q._mq = _mm_mul_ps(q._mq, _mm_set1_ps(mag));
		}

		return q;
	}

	Vec3 Quat::cross(const Quat& q) const
	{
		return Vec3(
			(this->_qy * q._qz) - (this->_qz * q._qy),
			(this->_qz * q._qx) - (this->_qx * q._qz),
			(this->_qx * q._qy) - (this->_qy * q._qx)
		);
	}

	float Quat::dot(const Quat& q) const
	{
		return float(
			(this->_qx * q._qx) + (this->_qy * q._qy) + (this->_qz * q._qz) + (this->_qw * q._qw)
		);
	}

	Quat Quat::getT() const
	{
		return Quat(-this->_qx, -this->_qy, -this->_qz, this->_qw);
	}

	Quat& Quat::T()
	{
		this->_mq = _mm_set_ps(this->_qw, -this->_qz, -this->_qy, -this->_qx);
		return *this;
	}

	const QuatProxy Quat::mag() const
	{
		return QuatProxy(this->_qx, this->_qy, this->_qz, this->_qw);
	}

	float Quat::magSquared() const
	{
		return float(
			(this->_qx * this->_qx) + (this->_qy * this->_qy) + (this->_qz * this->_qz) + (this->_qw * this->_qw)
			);
	}

	float Quat::invMag() const
	{
		return float(1.0f / Trig::sqrt(((this->_qx * this->_qx) + (this->_qy * this->_qy) + (this->_qz * this->_qz + this->_qw * this->_qw))));
	}

	Quat Quat::getInv() const
	{
		assert(!this->isZero());

		Quat temp = *this;
		return temp.getConj() / temp.magSquared();
	}

	Quat& Quat::inv()
	{
		assert(!this->isZero());

		*this = this->getConj() / this->magSquared();

		return *this;
	}

	Quat& Quat::conj()
	{
		this->_mq = _mm_set_ps(this->_qw, -this->_qz, -this->_qy, -this->_qx);
		return *this;
	}

	Quat Quat::getConj() const
	{
		return Quat( -this->_qx, -this->_qy, -this->_qz, this->_qw);
	}

	Quat Quat::getNeg() const
	{
		return Quat(-this->_qx, -this->_qy, -this->_qz, -this->_qw);
	}

	void Quat::getVec3(Vec3& vOut)
	{
		vOut = this->_qV3;
	}

	float Quat::getAngle() const
	{
		return 2 * (Trig::acos(this->getNorm()._qw));
	}

	void Quat::getAxis(Vec3& vOut)
	{
		vOut = this->_qV3.getNorm();
	}

	void Quat::Lqcvq(const Vec3& vIn, Vec3& vOut) const
	{
		vOut = (this->_qV3.cross(vIn)) * (2.0f * this->_qw);
		vOut += vIn * (this->_qw * this->_qw - this->_qV3.dot(this->_qV3));
		vOut += this->_qV3 * (2.0f * this->_qV3.dot(vIn));
	}

	void Quat::Lqvqc(const Vec3& vIn, Vec3& vOut) const
	{
		vOut = (vIn.cross(this->_qV3)) * (2.0f * this->_qw);
		vOut += vIn * (this->_qw * this->_qw - this->_qV3.dot(this->_qV3));
		vOut += this->_qV3 * (2.0f * this->_qV3.dot(vIn));
	}

	const bool Quat::isEqual(const Quat& q, const float epsilon) const
	{
		//using fabs
		float xEqual = (float)fabs(this->_qx - q._qx);
		float yEqual = (float)fabs(this->_qy - q._qy);
		float zEqual = (float)fabs(this->_qz - q._qz);
		float wEqual = (float)fabs(this->_qw - q._qw);

		return xEqual <= epsilon &&
			yEqual <= epsilon &&
			zEqual <= epsilon &&
			wEqual <= epsilon;
	}
	
	const bool Quat::isEquivalent(const Quat& q, const float epsilon) const
	{
		return (this->isEqual(q, epsilon) || this->getNeg().isEqual(q, epsilon));
	}
	
	const bool Quat::isNegEqual(const Quat& q, const float epsilon) const
	{
		return (this->getNeg().isEqual(q, epsilon));
	}
	
	const bool Quat::isConjugateEqual(const Quat& q, const float epsilon) const
	{
		return (this->getConj().isEqual(q, epsilon));
	}
	
	const bool Quat::isIdentity(const float epsilon) const
	{
		return (this->_qx >= 0.0f && this->_qx <= (0.0f + epsilon)) &&
			(this->_qy >= 0.0f && this->_qy <= (0.0f + epsilon)) &&
			(this->_qz >= 0.0f && this->_qz <= (0.0f + epsilon)) &&
			(this->_qw >= 1.0f && this->_qw <= (1.0f + epsilon));
	}
	
	const bool Quat::isNormalized(const float epsilon) const
	{
		float mag = this->magSquared();
		return (mag <= (1.0f + epsilon) && mag >= (1.0f - epsilon));
	}
	
	const bool Quat::isZero(const float epsilon) const
	{
		return (this->_qx >= 0.0f && this->_qx <= (0.0f + epsilon)) &&
			(this->_qy >= 0.0f && this->_qy <= (0.0f + epsilon)) &&
			(this->_qz >= 0.0f && this->_qz <= (0.0f + epsilon)) &&
			(this->_qw >= 0.0f && this->_qw <= (0.0f + epsilon));
	}

	void Quat::clear()
	{
		_mm_store_ps(&this->_qx, _mm_setzero_ps());
	}

	Quat& Quat::set(float x, float y, float z, float real)
	{
		this->_mq = _mm_set_ps(real, z, y, x);
		return *this;
	}

	Quat& Quat::set(Vec4& v)
	{
		_mm_store_ps(&this->_qx, v._mv);
		return *this;
	}

	Quat& Quat::set(Vec3& v, float real)
	{
		_mm_store_ps(&this->_qx, v._mv);
		this->_qw = real;
		return *this;
	}

	Quat& Quat::set(Mat4& m)
	{
		float tr = m._m0 + m._m5 + m._m10;
		float s;

		if (tr > 0.0f)
		{
			s = 2 * sqrtf(tr + 1.0f);
			this->_qw = 0.25f * s;
			this->_qx = -(m._m9 - m._m6) / s;
			this->_qy = -(m._m2 - m._m8) / s;
			this->_qz = -(m._m4 - m._m1) / s;
		}
		else if((m._m0 > m._m5) && (m._m0 > m._m10))
		{
			s = 2 * sqrtf(1.0f + m._m0 - m._m5 - m._m10);
			this->_qw = -(m._m9 - m._m6) / s;
			this->_qx = 0.25f * s;
			this->_qy = (m._m1 + m._m4) / s;
			this->_qz = (m._m2 + m._m8) / s;
		}
		else if(m._m5 > m._m10)
		{
			s = 2 * sqrtf(1.0f + m._m5 - m._m0 - m._m10);
			this->_qw = -(m._m2 - m._m8) / s;
			this->_qx = (m._m1 + m._m4) / s;
			this->_qy = 0.25f * s;
			this->_qz = (m._m6 + m._m9) / s;
		}
		else
		{
			s = 2 * sqrtf(1.0f + m._m10 - m._m0 - m._m5);
			this->_qw = -(m._m4 - m._m1) / s;
			this->_qx = (m._m2 + m._m8) / s;
			this->_qy = (m._m6 + m._m9) / s;
			this->_qz = 0.25f * s;
		}

		return *this;
	}

	Quat& Quat::set(Quat::Rot1 rot, float angle)
	{
		_mm_store_ps(&this->_qx, _mm_setzero_ps());
		float halfAngle = angle * 0.5f;
		this->_qw = Trig::cos(halfAngle);

		switch (rot)
		{
		case Quat::Rot1::X:
		{
			this->_qx = Trig::sin(halfAngle);
			break;
		}
		case Quat::Rot1::Y:
		{
			this->_qy = Trig::sin(halfAngle);
			break;
		}
		case Quat::Rot1::Z:
		{
			this->_qz = Trig::sin(halfAngle);
			break;
		}
		default:
			assert(false);
			break;
		}

		return *this;
	}

	Quat& Quat::set(const Quat::Rot3, const float x, const float y, const float z)
	{
		Quat qx(Quat::Rot1::X, x);
		Quat qy(Quat::Rot1::Y, y);
		Quat qz(Quat::Rot1::Z, z);

		*this = qx * qy * qz;

		return *this;
	}

	Quat& Quat::set(const Quat::Orient type, const Vec3& dof, const Vec3& up)
	{
		switch (type)
		{
			case Quat::Orient::LocalToWorld: {
				*this = Quat(Mat4(Mat4::Orient::LocalToWorld, dof, up));
				break;
			}
			case Quat::Orient::WorldToLocal: {
				*this = Quat(Mat4(Mat4::Orient::WorldToLocal, dof, up));
				break;
			}
			default:
				assert(false);
		}

		return *this;
	}

	Quat& Quat::setVec3(const Vec3& v)
	{
		this->_qx = v._vx;
		this->_qy = v._vy;
		this->_qz = v._vz;

		return *this;
	}

	Quat& Quat::set(const Quat::Rot, const Vec3& axis, const float angle)
	{

		const float a = 0.5f * angle;
		float cos = cosf(a);
		float sin = sinf(a);

		Vec3 v = axis.getNorm();
		v *= sin;
		
		this->set(v, cos);
		
		return *this;
	}

	Quat& Quat::set(const Quat::Special type)
	{
		switch (type)
		{
		case Quat::Special::Zero:
		{
			_mm_store_ps(&this->_qx, _mm_setzero_ps());
			break;
		}
		case Quat::Special::Identity:
		{
			_mm_store_ps(&this->_qx, _mm_setzero_ps());
			this->_qw = 1.0f;
			break;
		}
		default:
			assert(false);
		}

		return *this;
	}

	Quat Quat::operator+(const Quat& q) const
	{
		Quat a;
		a._mq = _mm_add_ps(this->_mq, q._mq);

		return a;
	}

	Quat& Quat::operator+=(const Quat& q)
	{
		this->_mq = _mm_add_ps(this->_mq, q._mq);

		return *this;
	}

	Quat Quat::operator-(const Quat& q) const
	{
		Quat a;
		a._mq = _mm_sub_ps(this->_mq, q._mq);

		return a;
	}

	Quat& Quat::operator-=(const Quat& q)
	{
		this->_mq = _mm_sub_ps(this->_mq, q._mq);
		return *this;
	}

	Quat Quat::operator*(const Quat& q) const
	{
		return Quat(
			(q.cross(*this) + q._qw * this->_qV3 + this->_qw * q._qV3),
			(this->_qw * q._qw - this->_qV3.dot(q._qV3))
		);
	}

	Quat& Quat::operator*=(const Quat& q)
	{
		Vec3 v = Vec3(q.cross(*this) + q._qw * this->_qV3 + this->_qw * q._qV3);

		this->set(
			v,
			(this->_qw * q._qw - this->_qV3.dot(q._qV3))
		);

		return *this;
	}

	Quat Quat::operator/(const Quat& q) const
	{
		Quat ret;
		ret._mq = _mm_div_ps(this->_mq, q._mq);
		return ret;
	}

	Quat& Quat::operator/=(const Quat& q)
	{
		this->_mq = _mm_div_ps(this->_mq, q._mq);
		return *this;
	}

	bool Quat::operator==(const Quat& q) const
	{
		return(
			this->_qV4 == q._qV4
		);
	}

	bool Quat::operator==(const Vec4& v) const
	{
		return(
			this->_qV4 == v
		);
	}

	Quat Quat::operator+() const
	{
		return(*this);
	}

	Quat Quat::operator-() const
	{
		Quat q;
		q._mq = _mm_sub_ps(_mm_setzero_ps(), this->_mq);
		return q;
	}

	Quat Quat::operator+(const float& f) const
	{
		Quat q;
		q._mq = _mm_add_ps(this->_mq, _mm_set1_ps(f));
		return (q);
	}

	Quat& Quat::operator+=(const float& f)
	{
		this->_mq = _mm_add_ps(this->_mq, _mm_set1_ps(f));
		return *this;
	}

	Quat Quat::operator-(const float& f) const
	{
		Quat q;
		q._mq = _mm_sub_ps(this->_mq, _mm_set1_ps(f));
		return (q);
	}

	Quat& Quat::operator-=(const float& f)
	{
		this->_mq = _mm_sub_ps(this->_mq, _mm_set1_ps(f));
		return *this;
	}

	Quat Quat::operator*(const float& f) const
	{
		Quat q;
		q._mq = _mm_mul_ps(this->_mq, _mm_set1_ps(f));
		return (q);
	}

	Quat& Quat::operator*=(const float& f)
	{
		this->_mq = _mm_mul_ps(this->_mq, _mm_set1_ps(f));
		return *this;
	}

	Quat Quat::operator/(const float& f) const
	{
		Quat q;
		q._mq = _mm_div_ps(this->_mq, _mm_set1_ps(f));
		return (q);
	}

	Quat& Quat::operator/=(const float& f)
	{
		this->_mq = _mm_div_ps(this->_mq, _mm_set1_ps(f));
		return *this;
	}

	Quat operator+(float f, const Quat& q)
	{
		Quat ret;
		ret._mq = _mm_add_ps(_mm_set1_ps(f), q._mq);
		return ret;
	}

	Quat operator-(float f, const Quat& q)
	{
		Quat ret;
		ret._mq = _mm_sub_ps(_mm_set1_ps(f), q._mq);
		return ret;
	}

	Quat operator*(float f, const Quat& q)
	{
		Quat ret;
		ret._mq = _mm_mul_ps(_mm_set1_ps(f), q._mq);
		return ret;
	}

	Quat operator/(float f, const Quat& q)
	{
		Quat ret;
		ret._mq = _mm_div_ps(_mm_set1_ps(f), q._mq);
		return ret;
	}

	Quat Quat::operator*(const Mat4& m) const
	{
		return (*this * Quat(m));
	}

	Quat& Quat::operator*=(const Mat4& m)
	{
		*this *= Quat(m);

		return *this;
	}

	Quat& Quat::operator=(const Mat4& m)
	{
		float tr = m._m0 + m._m5 + m._m10;
		float s;

		if (tr > 0.0f)
		{
			s = 2 * sqrtf(tr + 1.0f);
			this->_qw = 0.25f * s;
			this->_qx = -(m._m9 - m._m6) / s;
			this->_qy = -(m._m2 - m._m8) / s;
			this->_qz = -(m._m4 - m._m1) / s;
		}
		else if ((m._m0 > m._m5) && (m._m0 > m._m10))
		{
			s = 2 * sqrtf(1.0f + m._m0 - m._m5 - m._m10);
			this->_qw = -(m._m9 - m._m6) / s;
			this->_qx = 0.25f * s;
			this->_qy = (m._m1 + m._m4) / s;
			this->_qz = (m._m2 + m._m8) / s;
		}
		else if (m._m5 > m._m10)
		{
			s = 2 * sqrtf(1.0f + m._m5 - m._m0 - m._m10);
			this->_qw = -(m._m2 - m._m8) / s;
			this->_qx = (m._m1 + m._m4) / s;
			this->_qy = 0.25f * s;
			this->_qz = (m._m6 + m._m9) / s;
		}
		else
		{
			s = 2 * sqrtf(1.0f + m._m10 - m._m0 - m._m5);
			this->_qw = -(m._m4 - m._m1) / s;
			this->_qx = (m._m2 + m._m8) / s;
			this->_qy = (m._m6 + m._m9) / s;
			this->_qz = 0.25f * s;
		}

		return *this;
	}
}



//--- End of File ---
