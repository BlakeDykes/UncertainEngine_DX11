//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Uncertain
{
	// Do your magic here
	Mat4 Mat4::IDENTITY_MAT4(1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	Mat4 Mat4::ZERO_MAT4(0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	Mat4::Mat4()
	{
		_mm_store_ps(&this->_v0._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v1._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v2._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v3._vx, _mm_setzero_ps());
	}

	Mat4::Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d)
	{
		_mm_store_ps(&this->_v0._vx, a._mv);
		_mm_store_ps(&this->_v1._vx, b._mv);
		_mm_store_ps(&this->_v2._vx, c._mv);
		_mm_store_ps(&this->_v3._vx, d._mv);
	}

	Mat4::Mat4(float m0, float m1, float m2, float m3,
		float m4, float m5, float m6, float m7,
		float m8, float m9, float m10, float m11,
		float m12, float m13, float m14, float m15)
		: _m0(m0), _m1(m1), _m2(m2), _m3(m3),
		_m4(m4), _m5(m5), _m6(m6), _m7(m7),
		_m8(m8), _m9(m9), _m10(m10), _m11(m11),
		_m12(m12), _m13(m13), _m14(m14), _m15(m15)
	{
		this->SetGeneralHint();
	}

	Mat4::Mat4(Mat4::Orient orient, Vec3 dof, Vec3 up)
		:_m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
		Vec3 z = dof;
		Vec3 x = up.cross(z);
		Vec3 y = z.cross(x);

		x.norm();
		y.norm();
		z.norm();

		switch (orient)
		{
		case Mat4::Orient::LocalToWorld:
			this->_v0._mv = _mm_set_ps(0.0f, x._vz, x._vy, x._vx);
			this->_v1._mv = _mm_set_ps(0.0f, y._vz, y._vy, y._vx);
			this->_v2._mv = _mm_set_ps(0.0f, z._vz, z._vy, z._vx);
			break;
		case Mat4::Orient::WorldToLocal:
			this->_v0._mv = _mm_set_ps(0.0f, z._vx, y._vx, x._vx);
			this->_v1._mv = _mm_set_ps(0.0f, z._vy, y._vy, x._vy);
			this->_v2._mv = _mm_set_ps(0.0f, z._vz, y._vz, x._vz);
			break;
		default:
			assert(false);
			break;
		}

		this->SetRotHint();
	}

	Mat4::Mat4(Mat4::Rot axis, const Vec3& v, const float radians)
		:_m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
		assert(axis == Mat4::Rot::AxisAngle);
		AZUL_UNUSED_VAR(axis);

		const float angle_a = 0.5f * radians;
		float cos_a;
		float sin_a;

		cos_a = cosf(angle_a);
		sin_a = sinf(angle_a);

		Vec3 qV = v;
		qV.norm();

		qV *= sin_a;

		Vec4 Q;
		Q._mv = _mm_set_ps(cos_a, qV[z], qV[y], qV[x]);

		// this function has been transposed
		float x2, y2, z2;
		float xx, xy, xz;
		float yy, yz, zz;
		float wx, wy, wz;

		// ADD test to make sure that quat is normalized

		x2 = Q[x] + Q[x];
		y2 = Q[y] + Q[y];
		z2 = Q[z] + Q[z];

		xx = Q[x] * x2;
		xy = Q[x] * y2;
		xz = Q[x] * z2;

		yy = Q[y] * y2;
		yz = Q[y] * z2;
		zz = Q[z] * z2;

		wx = Q[w] * x2;
		wy = Q[w] * y2;
		wz = Q[w] * z2;

		this->_v0._mv = _mm_set_ps(0.0f, xz - wy, xy + wz, 1.0f - (yy + zz));
		this->_v1._mv = _mm_set_ps(0.0f, yz + wx, 1.0f - (xx + zz), xy - wz);
		this->_v2._mv = _mm_set_ps(0.0f, 1.0f - (xx + yy), yz - wx, xz + wy);

		this->SetRotHint();

	}

	Mat4::Mat4(Mat4::Rot1 rot, float angle)
		:_m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
		float cos_a = cosf(angle);
		float sin_a = sinf(angle);

		switch (rot)
		{
		case Mat4::Rot1::X:
			this->_v0._mv = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
			this->_v1._mv = _mm_set_ps(0.0f, sin_a, cos_a, 0.0f);
			this->_v2._mv = _mm_set_ps(0.0f, cos_a, -sin_a, 0.0f);
			break;
		case Mat4::Rot1::Y:
			this->_v0._mv = _mm_set_ps(0.0f, -sin_a, 0.0f, cos_a);
			this->_v1._mv = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
			this->_v2._mv = _mm_set_ps(0.0f, cos_a, 0.0f, sin_a);
			break;
		case Mat4::Rot1::Z:
			this->_v0._mv = _mm_set_ps(0.0f, 0.0f, sin_a, cos_a);
			this->_v1._mv = _mm_set_ps(0.0f, 0.0f, cos_a, -sin_a);
			this->_v2._mv = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
			break;
		default:
			assert(false);
		}

		this->SetRotHint();

	}

	Mat4::Mat4(Mat4::Rot3 rot, float xAngle, float yAngle, float zAngle)
		: _m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
		assert(rot == Mat4::Rot3::XYZ);
		AZUL_UNUSED_VAR(rot);

		Mat4 zRot = Mat4(Mat4::Rot1::Z, zAngle);
		Mat4 yRot = Mat4(Mat4::Rot1::Y, yAngle);
		Mat4 xRot = Mat4(Mat4::Rot1::X, xAngle);

		*this = xRot * yRot * zRot;

		this->SetRotHint();
	}

	Mat4::Mat4(Mat4::Trans dir, float xDist, float yDist, float zDist)
		: _m0(1.0f), _m1(0.0f), _m2(0.0f), _m3(0.0f),
		_m4(0.0f), _m5(1.0f), _m6(0.0f), _m7(0.0f),
		_m8(0.0f), _m9(0.0f), _m10(1.0f), _m11(0.0f),
		_m12(xDist), _m13(yDist), _m14(zDist), _m15(1.0f)
	{
		assert(dir == Mat4::Trans::XYZ);
		AZUL_UNUSED_VAR(dir);

		this->SetTransHint();
	}

	Mat4::Mat4(Mat4::Scale scale, float xScale, float yScale, float zScale)
		: _m0(xScale), _m1(0.0f), _m2(0.0f), _m3(0.0f),
		_m4(0.0f), _m5(yScale), _m6(0.0f), _m7(0.0f),
		_m8(0.0f), _m9(0.0f), _m10(zScale), _m11(0.0f),
		_m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
		assert(scale == Mat4::Scale::XYZ);
		AZUL_UNUSED_VAR(scale);

		this->SetScaleHint();
	}

	Mat4::Mat4(Mat4::Scale scale, const Vec3& v)
		: _m0(v._vx), _m1(0.0f), _m2(0.0f), _m3(0.0f),
		_m4(0.0f), _m5(v._vy), _m6(0.0f), _m7(0.0f),
		_m8(0.0f), _m9(0.0f), _m10(v._vz), _m11(0.0f),
		_m12(0.0f), _m13(0.0f), _m14(0.0f), _m15(1.0f)
	{
		assert(scale == Mat4::Scale::XYZ);
		AZUL_UNUSED_VAR(scale);

		this->SetScaleHint();
	}

	Mat4::Mat4(Mat4::Special type)
	{
		switch (type)
		{
		case Mat4::Special::Identity:
			*this = IDENTITY_MAT4;
			break;
		case Mat4::Special::Zero:
			*this = ZERO_MAT4;
			break;
		}
	}

	Mat4::Mat4(Mat4::Trans type, const Vec3& v)
		: _m0(1.0f), _m1(0.0f), _m2(0.0f), _m3(0.0f),
		_m4(0.0f), _m5(1.0f), _m6(0.0f), _m7(0.0f),
		_m8(0.0f), _m9(0.0f), _m10(1.0f), _m11(0.0f),
		_m12(v._vx), _m13(v._vy), _m14(v._vz), _m15(1.0f)
	{
		assert(type == Mat4::Trans::XYZ);
		AZUL_UNUSED_VAR(type);

		this->SetTransHint();
	}

	Mat4::Mat4(const Quat& q)
	{
		Vec4 x, y, z;
		_mm_store_ps(&x._vx, _mm_set1_ps(q._qx));
		_mm_store_ps(&y._vx, _mm_set1_ps(q._qy));
		_mm_store_ps(&z._vx, _mm_set1_ps(q._qz));

		// xx, xy, xz, xw
		x._mv = _mm_mul_ps(x._mv, q._mq);
		// yx, yy, yz, yw
		y._mv = _mm_mul_ps(y._mv, q._mq);
		// zx, zy, zz, zw
		z._mv = _mm_mul_ps(z._mv, q._mq);

		*this = {
			(1.0f - 2.0f * (y._vy + z._vz)), 2.0f * (x._vy + z._vw),		2.0f * (x._vz - y._vw),			0.0f,
			2.0f * (x._vy - z._vw),			 1.0f - 2.0f * (x._vx + z._vz), 2.0f * (y._vz + x._vw),			0.0f,
			2.0f * (x._vz + y._vw),			 2.0f * (y._vz - x._vw),		1.0f - 2.0f * (x._vx + y._vy),	0.0f,
			0.0f,							 0.0f,							0.0f,							1.0f
		};
	}

	Mat4::Mat4(const Mat4& m)
	{
		_mm_store_ps(&this->_v0._vx, m._v0._mv);
		_mm_store_ps(&this->_v1._vx, m._v1._mv);
		_mm_store_ps(&this->_v2._vx, m._v2._mv);
		_mm_store_ps(&this->_v3._vx, m._v3._mv);
	}

	Mat4& Mat4::operator =(const Mat4& m)
	{
		_mm_store_ps(&this->_v0._vx, m._v0._mv);
		_mm_store_ps(&this->_v1._vx, m._v1._mv);
		_mm_store_ps(&this->_v2._vx, m._v2._mv);
		_mm_store_ps(&this->_v3._vx, m._v3._mv);

		return *this;
	}

	Mat4& Mat4::operator =(const Mat3& m)
	{
		this->_v0._mv = _mm_set_ps(this->_m3, m._m2, m._m1, m._m0);
		this->_v1._mv = _mm_set_ps(this->_m7, m._m6, m._m5, m._m4);
		this->_v2._mv = _mm_set_ps(this->_m11, m._m10, m._m9, m._m8);

		return *this;
	}

	Mat4& Mat4::set(Mat4::Special type)
	{
		switch (type)
		{
		case Mat4::Special::Identity:
			*this = Mat4::IDENTITY_MAT4;
			break;
		case Mat4::Special::Zero:
			*this = Mat4::ZERO_MAT4;
			break;
		}

		return *this;
	}

	Mat4& Mat4::set(Mat4::Trans type, float x, float y, float z)
	{
		assert(type == Mat4::Trans::XYZ);
		AZUL_UNUSED_VAR(type);

		this->_v0._mv = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
		this->_v1._mv = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
		this->_v2._mv = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
		this->_v3._mv = _mm_set_ps(1.0f, z, y, x);

		this->SetTransHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Trans type, const Vec3& v)
	{
		assert(type == Mat4::Trans::XYZ);
		AZUL_UNUSED_VAR(type);

		this->_v0._mv = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
		this->_v1._mv = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
		this->_v2._mv = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
		this->_v3._mv = _mm_set_ps(1.0f, v._vz, v._vy, v._vx);

		this->SetTransHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Scale type, float x, float y, float z)
	{
		assert(type == Mat4::Scale::XYZ);
		AZUL_UNUSED_VAR(type);

		this->_v0._mv = _mm_set_ps(0.0f, 0.0f, 0.0f, x);
		this->_v1._mv = _mm_set_ps(0.0f, 0.0f, y, 0.0f);
		this->_v2._mv = _mm_set_ps(0.0f, z, 0.0f, 0.0f);
		this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

		this->SetScaleHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Scale type, const Vec3& v)
	{
		assert(type == Mat4::Scale::XYZ);
		AZUL_UNUSED_VAR(type);

		this->_v0._mv = _mm_set_ps(0.0f, 0.0f, 0.0f, v._vx);
		this->_v1._mv = _mm_set_ps(0.0f, 0.0f, v._vy, 0.0f);
		this->_v2._mv = _mm_set_ps(0.0f, v._vz, 0.0f, 0.0f);
		this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

		this->SetScaleHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Rot1 rot, float radians)
	{
		float cos_a = cosf(radians);
		float sin_a = sinf(radians);

		switch (rot)
		{
		case Mat4::Rot1::X:
			this->_v0._mv = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
			this->_v1._mv = _mm_set_ps(0.0f, sin_a, cos_a, 0.0f);
			this->_v2._mv = _mm_set_ps(0.0f, cos_a, -sin_a, 0.0f);
			this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			break;
		case Mat4::Rot1::Y:
			this->_v0._mv = _mm_set_ps(0.0f, -sin_a, 0.0f, cos_a);
			this->_v1._mv = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
			this->_v2._mv = _mm_set_ps(0.0f, cos_a, 0.0f, sin_a);
			this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			break;
		case Mat4::Rot1::Z:
			this->_v0._mv = _mm_set_ps(0.0f, 0.0f, sin_a, cos_a);
			this->_v1._mv = _mm_set_ps(0.0f, 0.0f, cos_a, -sin_a);
			this->_v2._mv = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
			this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			break;
		default:
			assert(false);
		}

		this->SetRotHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Rot3 axis, float xAngle, float yAngle, float zAngle)
	{
		assert(axis == Mat4::Rot3::XYZ);
		AZUL_UNUSED_VAR(axis);

		Mat4 zRot = Mat4(Mat4::Rot1::Z, zAngle);
		Mat4 yRot = Mat4(Mat4::Rot1::Y, yAngle);
		Mat4 xRot = Mat4(Mat4::Rot1::X, xAngle);

		*this = xRot * yRot * zRot;

		this->SetRotHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Rot axis, const Vec3& v, const float radians)
	{
		assert(axis == Mat4::Rot::AxisAngle);
		AZUL_UNUSED_VAR(axis);

		const float angle_a = 0.5f * radians;
		float cos_a;
		float sin_a;

		cos_a = cosf(angle_a);
		sin_a = sinf(angle_a);

		Vec3 qV = v;
		qV.norm();

		qV *= sin_a;

		Vec4 Q;
		Q._mv = _mm_set_ps(cos_a, qV[z], qV[y], qV[x]);

		// this function has been transposed
		float x2, y2, z2;
		float xx, xy, xz;
		float yy, yz, zz;
		float wx, wy, wz;

		// ADD test to make sure that quat is normalized

		x2 = Q[x] + Q[x];
		y2 = Q[y] + Q[y];
		z2 = Q[z] + Q[z];

		xx = Q[x] * x2;
		xy = Q[x] * y2;
		xz = Q[x] * z2;

		yy = Q[y] * y2;
		yz = Q[y] * z2;
		zz = Q[z] * z2;

		wx = Q[w] * x2;
		wy = Q[w] * y2;
		wz = Q[w] * z2;

		this->_v0._mv = _mm_set_ps(0.0f, xz - wy, xy + wz, 1.0f - (yy + zz));
		this->_v1._mv = _mm_set_ps(0.0f, yz + wx, 1.0f - (xx + zz), xy - wz);
		this->_v2._mv = _mm_set_ps(0.0f, 1.0f - (xx + yy), yz - wx, xz + wy);
		this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);

		this->SetRotHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Row row, const Vec3& v)
	{
		switch (row)
		{
		case Mat4::Row::i0:
			this->_v0 = Vec4(v, 0.0f);
			break;
		case Mat4::Row::i1:
			this->_v1 = Vec4(v, 0.0f);
			break;
		case Mat4::Row::i2:
			this->_v2 = Vec4(v, 0.0f);
			break;
		case Mat4::Row::i3:
			this->_v3 = Vec4(v, 0.0f);
			break;
		default:
			assert(false);
			break;
		}

		this->SetGeneralHint();

		return *this;
	}


	Mat4& Mat4::set(Mat4::Row row, const Vec4& v)
	{
		switch (row)
		{
		case Mat4::Row::i0:
			this->_v0 = v;
			break;
		case Mat4::Row::i1:
			this->_v1 = v;
			break;
		case Mat4::Row::i2:
			this->_v2 = v;
			break;
		case Mat4::Row::i3:
			this->_v3 = v;
			break;
		default:
			assert(false);
			break;
		}

		this->SetGeneralHint();

		return *this;
	}

	Mat4& Mat4::set(Mat4::Orient orient, const Vec3 dof, const Vec3 up)
	{
		Vec3 z = dof;
		Vec3 x = up.cross(z);
		Vec3 y = z.cross(x);

		x.norm();
		y.norm();
		z.norm();

		switch (orient)
		{
		case Mat4::Orient::LocalToWorld:
			this->_v0._mv = _mm_set_ps(0.0f, x._vz, x._vy, x._vx);
			this->_v1._mv = _mm_set_ps(0.0f, y._vz, y._vy, y._vx);
			this->_v2._mv = _mm_set_ps(0.0f, z._vz, z._vy, z._vx);
			this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			break;
		case Mat4::Orient::WorldToLocal:
			this->_v0._mv = _mm_set_ps(0.0f, z._vx, y._vx, x._vx);
			this->_v1._mv = _mm_set_ps(0.0f, z._vy, y._vy, x._vy);
			this->_v2._mv = _mm_set_ps(0.0f, z._vz, y._vz, x._vz);
			this->_v3._mv = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
			break;
		default:
			assert(false);
			break;
		}

		this->SetRotHint();

		return *this;
	}

	Mat4& Mat4::set(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d)
	{
		this->_v0._mv = _mm_set_ps(0.0f, a._vz, a._vy, a._vx);
		this->_v1._mv = _mm_set_ps(0.0f, b._vz, b._vy, b._vx);
		this->_v2._mv = _mm_set_ps(0.0f, c._vz, c._vy, c._vx);
		this->_v3._mv = _mm_set_ps(1.0f, d._vz, d._vy, d._vx);

		this->SetGeneralHint();

		return *this;
	}

	Mat4& Mat4::set(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& d)
	{
		_mm_store_ps(&this->_v0._vx, a._mv);
		_mm_store_ps(&this->_v1._vx, b._mv);
		_mm_store_ps(&this->_v2._vx, c._mv);
		_mm_store_ps(&this->_v3._vx, d._mv);

		return *this;
	}

	Mat4& Mat4::set(const Quat& q)
	{
		Vec4 x, y, z;
		_mm_store_ps(&x._vx, _mm_set1_ps(q._qx));
		_mm_store_ps(&y._vx, _mm_set1_ps(q._qy));
		_mm_store_ps(&z._vx, _mm_set1_ps(q._qz));

		// xx, xy, xz, xw
		x._mv = _mm_mul_ps(x._mv, q._mq);
		// yx, yy, yz, yw
		y._mv = _mm_mul_ps(y._mv, q._mq);
		// zx, zy, zz, zw
		z._mv = _mm_mul_ps(z._mv, q._mq);

		*this = {
			(1.0f - 2.0f * (y._vy + z._vz)), 2.0f * (x._vy + z._vw),		2.0f * (x._vz - y._vw),			0.0f,
			2.0f * (x._vy - z._vw),			 1.0f - 2.0f * (x._vx + z._vz), 2.0f * (y._vz + x._vw),			0.0f,
			2.0f * (x._vz + y._vw),			 2.0f * (y._vz - x._vw),		1.0f - 2.0f * (x._vx + y._vy),	0.0f,
			0.0f,							 0.0f,							0.0f,							1.0f
		};
		
		return *this;
	}

	void Mat4::clear()
	{
		_mm_store_ps(&this->_v0._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v1._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v2._vx, _mm_setzero_ps());
		_mm_store_ps(&this->_v3._vx, _mm_setzero_ps());
	}

	Vec4& Mat4::get(Mat4::Row row)
	{

		switch (row)
		{
		case Mat4::Row::i0:
			return this->_v0;
			break;
		case Mat4::Row::i1:
			return this->_v1;
			break;
		case Mat4::Row::i2:
			return this->_v2;
			break;
		case Mat4::Row::i3:
			return this->_v3;
			break;
		default:
			assert(false);
			return this->_v0;
			break;
		}
	}

	const float Mat4::det() const
	{
		float ta, tb, tc, td, te, tf;

		ta = (this->_m10 * this->_m15) - (this->_m11 * this->_m14);
		tb = (this->_m9 * this->_m15) - (this->_m11 * this->_m13);
		tc = (this->_m9 * this->_m14) - (this->_m10 * this->_m13);
		td = (this->_m8 * this->_m15) - (this->_m11 * this->_m12);
		te = (this->_m8 * this->_m13) - (this->_m9 * this->_m12);
		tf = (this->_m8 * this->_m14) - (this->_m10 * this->_m12);

		return ((this->_m0 * ((this->_m5 * ta) - (this->_m6 * tb) + (this->_m7 * tc)))
			- (this->_m1 * ((this->_m4 * ta) - (this->_m6 * td) + (this->_m7 * tf)))
			+ (this->_m2 * ((this->_m4 * tb) - (this->_m5 * td) + (this->_m7 * te)))
			- (this->_m3 * ((this->_m4 * tc) - (this->_m5 * tf) + (this->_m6 * te))));
	}

	Mat4& Mat4::adj()
	{
		float A, B, C, D, E, F, G, H, _I;
		Mat4 tmp;

		A = (_m10 * _m15) - (_m11 * _m14);
		B = (_m7 * _m14) - (_m6 * _m15);
		C = (_m6 * _m11) - (_m7 * _m10);
		D = (_m3 * _m14) - (_m2 * _m15);
		E = (_m2 * _m11) - (_m3 * _m10);
		F = (_m2 * _m7) - (_m3 * _m6);
		G = _m5;
		H = _m13;

		/* 0		a = {5,  9, 13,  6, 10, 14,  7, 11, 15} */
		tmp._m0 = (G * A) + (_m9 * B) + (H * C);

		/* 2		a = {1,  5, 13,  2,  6, 14,  3,  7, 15} */
		tmp._m2 = -(_m1 * B) + (G * D) + (H * F);

		G = _m0;
		H = _m8;
		/* 5		a = {0,  8, 12,  2, 10, 14,  3, 11, 15} */
		tmp._m5 = (G * A) + (H * D) + (_m12 * E);

		/* 7		a = {0,  4,  8,  2,  6, 10,  3,  7, 11} */
		tmp._m7 = (G * C) + (H * F) - (_m4 * E);

		A = (_m9 * _m15) - (_m11 * _m13);
		B = (_m3 * _m13) - (_m1 * _m15);
		C = (_m1 * _m11) - (_m3 * _m9);
		D = (_m7 * _m13) - (_m5 * _m15);
		E = (_m5 * _m11) - (_m7 * _m9);
		F = (_m1 * _m7) - (_m3 * _m5);
		G = _m2;
		H = _m10;
		/* 1		a = {2, 10, 14,  1,  9, 13,  3, 11, 15} */
		tmp._m1 = (G * A) + (H * B) + (_m14 * C);

		/* 3		a = {2,  6, 10,  1,  5,  9,  3,  7, 11} */
		tmp._m3 = (G * E) + (H * F) - (_m6 * C);

		G = _m4;
		H = _m12;
		/* 8		a = {4,  8, 12,  5,  9, 13,  7, 11, 15} */
		tmp._m8 = (G * A) + (_m8 * D) + (H * E);

		/* A		a = {0,  4, 12,  1,  5, 13,  3,  7, 15} */
		tmp._m10 = -(_m0 * D) + (G * B) + (H * F);

		A = (_m8 * _m15) - (_m11 * _m12);
		B = (_m4 * _m11) - (_m7 * _m8);
		C = (_m3 * _m12) - (_m0 * _m15);
		D = (_m0 * _m7) - (_m3 * _m4);
		E = (_m7 * _m12) - (_m4 * _m15);
		F = (_m0 * _m11) - (_m3 * _m8);
		G = _m6;
		H = _m14;
		/* 4		a = {6, 10, 14,  4,  8, 12,  7, 11, 15} */
		tmp._m4 = (G * A) + (_m10 * E) + (H * B);

		/* 6		a = {2,  6, 14,  0,  4, 12,  3,  7, 15} */
		tmp._m6 = -(_m2 * E) + (G * C) + (H * D);

		G = _m1;
		H = _m9;
		/* 9		a = {1,  9, 13,  0,  8, 12,  3, 11, 15} */
		tmp._m9 = (G * A) + (H * C) + (_m13 * F);

		/* B		a = {1,  5,  9,  0,  4,  8,  3,  7, 11} */
		tmp._m11 = (G * B) + (H * D) - (_m5 * F);

		A = _m4;
		B = _m6;
		C = _m12;
		D = _m14;
		E = (B * C) - (A * D);
		F = _m8;
		G = _m10;
		H = _m5;
		_I = _m13;
		/* C		a = {5,  9, 13,  4,  8, 12,  6, 10, 14} */
		tmp._m12 = (H * ((F * D) - (G * C))) + (_m9 * E) + (_I * ((A * G) - (B * F)));

		F = _m2;
		G = _m0;
		/* E		a = {1,  5, 13,  0,  4, 12,  2,  6, 14} */
		tmp._m14 = -(_m1 * E) + (H * ((F * C) - (G * D))) + (_I * ((G * B) - (F * A)));

		A = _m1;
		B = _m2;
		C = _m9;
		D = _m10;
		E = (A * D) - (B * C);
		F = _m13;
		G = _m14;
		H = _m0;
		_I = _m8;
		/* D		a = {0,  8, 12,  1,  9, 13,  2, 10, 14} */
		tmp._m13 = (H * ((C * G) - (D * F))) + (_I * ((B * F) - (A * G))) + (_m12 * E);

		F = _m5;
		G = _m6;
		/* F		a = {0,  4,  8,  1,  5,  9,  2,  6, 10}} */
		tmp._m15 = -(_m4 * E) + (H * ((F * D) - (G * C))) + (_I * ((A * G) - (B * F)));

		*this = tmp;

		return *this;
	}

	Mat4 Mat4::GetAdj() const
	{
		float A, B, C, D, E, F, G, H, _I;
		Mat4 tmp;

		A = (_m10 * _m15) - (_m11 * _m14);
		B = (_m7 * _m14) - (_m6 * _m15);
		C = (_m6 * _m11) - (_m7 * _m10);
		D = (_m3 * _m14) - (_m2 * _m15);
		E = (_m2 * _m11) - (_m3 * _m10);
		F = (_m2 * _m7) - (_m3 * _m6);
		G = _m5;
		H = _m13;

		/* 0		a = {5,  9, 13,  6, 10, 14,  7, 11, 15} */
		tmp._m0 = (G * A) + (_m9 * B) + (H * C);

		/* 2		a = {1,  5, 13,  2,  6, 14,  3,  7, 15} */
		tmp._m2 = -(_m1 * B) + (G * D) + (H * F);

		G = _m0;
		H = _m8;
		/* 5		a = {0,  8, 12,  2, 10, 14,  3, 11, 15} */
		tmp._m5 = (G * A) + (H * D) + (_m12 * E);

		/* 7		a = {0,  4,  8,  2,  6, 10,  3,  7, 11} */
		tmp._m7 = (G * C) + (H * F) - (_m4 * E);

		A = (_m9 * _m15) - (_m11 * _m13);
		B = (_m3 * _m13) - (_m1 * _m15);
		C = (_m1 * _m11) - (_m3 * _m9);
		D = (_m7 * _m13) - (_m5 * _m15);
		E = (_m5 * _m11) - (_m7 * _m9);
		F = (_m1 * _m7) - (_m3 * _m5);
		G = _m2;
		H = _m10;
		/* 1		a = {2, 10, 14,  1,  9, 13,  3, 11, 15} */
		tmp._m1 = (G * A) + (H * B) + (_m14 * C);

		/* 3		a = {2,  6, 10,  1,  5,  9,  3,  7, 11} */
		tmp._m3 = (G * E) + (H * F) - (_m6 * C);

		G = _m4;
		H = _m12;
		/* 8		a = {4,  8, 12,  5,  9, 13,  7, 11, 15} */
		tmp._m8 = (G * A) + (_m8 * D) + (H * E);

		/* A		a = {0,  4, 12,  1,  5, 13,  3,  7, 15} */
		tmp._m10 = -(_m0 * D) + (G * B) + (H * F);

		A = (_m8 * _m15) - (_m11 * _m12);
		B = (_m4 * _m11) - (_m7 * _m8);
		C = (_m3 * _m12) - (_m0 * _m15);
		D = (_m0 * _m7) - (_m3 * _m4);
		E = (_m7 * _m12) - (_m4 * _m15);
		F = (_m0 * _m11) - (_m3 * _m8);
		G = _m6;
		H = _m14;
		/* 4		a = {6, 10, 14,  4,  8, 12,  7, 11, 15} */
		tmp._m4 = (G * A) + (_m10 * E) + (H * B);

		/* 6		a = {2,  6, 14,  0,  4, 12,  3,  7, 15} */
		tmp._m6 = -(_m2 * E) + (G * C) + (H * D);

		G = _m1;
		H = _m9;
		/* 9		a = {1,  9, 13,  0,  8, 12,  3, 11, 15} */
		tmp._m9 = (G * A) + (H * C) + (_m13 * F);

		/* B		a = {1,  5,  9,  0,  4,  8,  3,  7, 11} */
		tmp._m11 = (G * B) + (H * D) - (_m5 * F);

		A = _m4;
		B = _m6;
		C = _m12;
		D = _m14;
		E = (B * C) - (A * D);
		F = _m8;
		G = _m10;
		H = _m5;
		_I = _m13;
		/* C		a = {5,  9, 13,  4,  8, 12,  6, 10, 14} */
		tmp._m12 = (H * ((F * D) - (G * C))) + (_m9 * E) + (_I * ((A * G) - (B * F)));

		F = _m2;
		G = _m0;
		/* E		a = {1,  5, 13,  0,  4, 12,  2,  6, 14} */
		tmp._m14 = -(_m1 * E) + (H * ((F * C) - (G * D))) + (_I * ((G * B) - (F * A)));

		A = _m1;
		B = _m2;
		C = _m9;
		D = _m10;
		E = (A * D) - (B * C);
		F = _m13;
		G = _m14;
		H = _m0;
		_I = _m8;
		/* D		a = {0,  8, 12,  1,  9, 13,  2, 10, 14} */
		tmp._m13 = (H * ((C * G) - (D * F))) + (_I * ((B * F) - (A * G))) + (_m12 * E);

		F = _m5;
		G = _m6;
		/* F		a = {0,  4,  8,  1,  5,  9,  2,  6, 10}} */
		tmp._m15 = -(_m4 * E) + (H * ((F * D) - (G * C))) + (_I * ((A * G) - (B * F)));

		return(tmp);
	}

	void Mat4::T()
	{
		__m128 vTemp1 = _mm_shuffle_ps(this->_v0._mv, this->_v1._mv, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp2 = _mm_shuffle_ps(this->_v2._mv, this->_v3._mv, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp3 = _mm_shuffle_ps(this->_v0._mv, this->_v1._mv, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 vTemp4 = _mm_shuffle_ps(this->_v2._mv, this->_v3._mv, _MM_SHUFFLE(3, 2, 3, 2));

		this->_v0._mv = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		this->_v1._mv = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		this->_v2._mv = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		this->_v3._mv = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));
	}

	Mat4 Mat4::getT() const
	{
		__m128 vTemp1 = _mm_shuffle_ps(this->_v0._mv, this->_v1._mv, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp2 = _mm_shuffle_ps(this->_v2._mv, this->_v3._mv, _MM_SHUFFLE(1, 0, 1, 0));
		__m128 vTemp3 = _mm_shuffle_ps(this->_v0._mv, this->_v1._mv, _MM_SHUFFLE(3, 2, 3, 2));
		__m128 vTemp4 = _mm_shuffle_ps(this->_v2._mv, this->_v3._mv, _MM_SHUFFLE(3, 2, 3, 2));

		Mat4 mRet;
		mRet._v0._mv = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(2, 0, 2, 0));
		mRet._v1._mv = _mm_shuffle_ps(vTemp1, vTemp2, _MM_SHUFFLE(3, 1, 3, 1));
		mRet._v2._mv = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(2, 0, 2, 0));
		mRet._v3._mv = _mm_shuffle_ps(vTemp3, vTemp4, _MM_SHUFFLE(3, 1, 3, 1));

		return mRet;
	}

	void Mat4::inv()
	{
		Hint h = this->GetHint();

		switch (h)
		{
		case Hint::Scale:
		{
			this->_m0 = 1 / this->_m0;
			this->_m5 = 1 / this->_m5;
			this->_m10 = 1 / this->_m10;
			break;
		}

		case Hint::Trans:
		{
			this->_v3.negateXYZ();
			break;
		}

		case Hint::TransScale:
		{
			this->_m0 = 1 / this->_m0;
			this->_m5 = 1 / this->_m5;
			this->_m10 = 1 / this->_m10;

			Mat3* pTemp = reinterpret_cast<Mat3*>(this);
			this->_v3.negateXYZ() *= pTemp->T();
			break;
		}

		case Hint::Rot:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(this);
			pTemp->T();
			break;
		}
		case Hint::RotScale:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(this);
			pTemp->inv();
			break;
		}

		case Hint::RotTrans:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(this);
			this->_v3.negateXYZ() *= pTemp->T();
			break;
		}

		case Hint::GeneralizedAffine:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(this);
			this->_v3.negateXYZ() *= pTemp->inv();
			break;
		}

		case Hint::GeneralizeMat4:
		{
			float det = this->det();
			if (fabs(det) >= MATH_TOLERANCE)
			{
				this->adj();
				*this /= det;
			}
			break;
		}
		default:
			assert(false);
		}
	}

	Mat4 Mat4::getInv() const
	{
		Hint h = this->GetHint();
		Mat4 mRet(*this);

		switch (h)
		{
		case Hint::Scale:
		{
			mRet._m0 = 1.0f / mRet._m0;
			mRet._m5 = 1.0f / mRet._m5;
			mRet._m10 = 1.0f / mRet._m10;
			break;
		}

		case Hint::Trans:
		{
			mRet._v3.negateXYZ();
			break;
		}

		case Hint::TransScale:
		{
			mRet._m0 = 1.0f / mRet._m0;
			mRet._m5 = 1.0f / mRet._m5;
			mRet._m10 = 1.0f / mRet._m10;

			Mat3* pTemp = reinterpret_cast<Mat3*>(&mRet);
			mRet._v3.negateXYZ() *= pTemp->T();
			break;
		}

		case Hint::Rot:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(&mRet);
			pTemp->T();
			break;
		}
		case Hint::RotScale:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(&mRet);
			pTemp->inv();
			break;
		}
		case Hint::RotTrans:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(&mRet);
			mRet._v3.negateXYZ() *= pTemp->T();
			break;
		}

		case Hint::GeneralizedAffine:
		{
			Mat3* pTemp = reinterpret_cast<Mat3*>(&mRet);
			mRet._v3.negateXYZ() *= pTemp->inv();
			break;
		}

		case Hint::GeneralizeMat4:
		{
			float det = mRet.det();
			if (fabs(det) >= MATH_TOLERANCE)
			{
				mRet.adj() /= det;
			}
			break;
		}
		default:
			assert(false);
		}

		return mRet;
	}

	bool Mat4::isIdentity(const float tolerance) const
	{
		// included in Vec4 operator ==
		AZUL_UNUSED_VAR(tolerance);

		return *this == Mat4::IDENTITY_MAT4;
	}

	bool Mat4::isIdentity() const
	{
		return *this == Mat4::IDENTITY_MAT4;
	}

	bool Mat4::isZero(const float tolerance) const
	{
		// included in Vec4 operator ==
		AZUL_UNUSED_VAR(tolerance);

		return *this == Mat4::ZERO_MAT4;
	}

	bool Mat4::isEqual(const Mat4& m)
	{
		return *this == m;
	}

	bool Mat4::isRotation() const
	{
		return (this->GetHint() == Hint::Rot);
	}


	const Mat4& Mat4::getIdentity()
	{
		return Mat4::IDENTITY_MAT4;
	}

	const Mat4& Mat4::getZero()
	{
		return Mat4::ZERO_MAT4;
	}

	void Mat4::m0(const float v) 
	{ 
		this->_m0 = v; 
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}

	void Mat4::m1(const float v)
	{ 
		this->_m1 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m2(const float v)
	{ 
		this->_m2 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m3(const float v)
	{ 
		this->_m3 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m4(const float v)
	{ 
		this->_m4 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m5(const float v)
	{ 
		this->_m5 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m6(const float v)
	{ 
		this->_m6 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m7(const float v)
	{ 
		this->_m7 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m8(const float v)
	{ 
		this->_m8 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m9(const float v)
	{ 
		this->_m9 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}

	void Mat4::m10(const float v) 
	{
		this->_m10 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m11(const float v) 
	{ 
		this->_m11 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}

	void Mat4::m12(const float v) 
	{ 
		this->_m12 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m13(const float v) 
	{ 
		this->_m13 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m14(const float v) 
	{ 
		this->_m14 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}
	
	void Mat4::m15(const float v) 
	{ 
		this->_m15 = v;
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;
	}

	Mat4 Mat4::operator+(const Mat4& m) const
	{
		Hint a = this->GetHint();
		Hint b = m.GetHint();

		Mat4 mRet;
		mRet._v0._mv = _mm_add_ps(this->_v0._mv, m._v0._mv);
		mRet._v1._mv = _mm_add_ps(this->_v1._mv, m._v1._mv);
		mRet._v2._mv = _mm_add_ps(this->_v2._mv, m._v2._mv);
		mRet._v3._mv = _mm_add_ps(this->_v3._mv, m._v3._mv);

		mRet.SetHint(a, b);

		return mRet;
	}

	Mat4 Mat4::operator-(const Mat4& m) const
	{
		Hint a = this->GetHint();
		Hint b = m.GetHint();

		Mat4 mRet;
		mRet._v0._mv = _mm_sub_ps(this->_v0._mv, m._v0._mv);
		mRet._v1._mv = _mm_sub_ps(this->_v1._mv, m._v1._mv);
		mRet._v2._mv = _mm_sub_ps(this->_v2._mv, m._v2._mv);
		mRet._v3._mv = _mm_sub_ps(this->_v3._mv, m._v3._mv);

		mRet.SetHint(a, b);

		return mRet;
	}

	Mat4 Mat4::operator*(const Mat4& m) const
	{

		Hint a = this->GetHint();
		Hint b = m.GetHint();

		Mat4 mRet;

		__m128 row1 = _mm_load_ps(&m._m0);
		__m128 row2 = _mm_load_ps(&m._m4);
		__m128 row3 = _mm_load_ps(&m._m8);
		__m128 row4 = _mm_load_ps(&m._m12);

		const float* pF = &this->_m0;
		float* pRetF = &mRet._m0;

		for (int i = 0; i < 4; i++)
		{
			__m128 x = _mm_set1_ps(pF[4 * i + 0]);
			__m128 y = _mm_set1_ps(pF[4 * i + 1]);
			__m128 z = _mm_set1_ps(pF[4 * i + 2]);
			__m128 w = _mm_set1_ps(pF[4 * i + 3]);
			__m128 row = _mm_add_ps(
				_mm_add_ps(_mm_mul_ps(x, row1), _mm_mul_ps(y, row2)),
				_mm_add_ps(_mm_mul_ps(z, row3), _mm_mul_ps(w, row4)));
			
			_mm_store_ps(&pRetF[4 * i], row);
		}

		mRet.SetHint(a, b);

		return mRet;
	}

	Mat4& Mat4::operator+=(const Mat4& m)
	{
		Hint a = this->GetHint();
		Hint b = m.GetHint();

		this->_v0._mv = _mm_add_ps(this->_v0._mv, m._v0._mv);
		this->_v1._mv = _mm_add_ps(this->_v1._mv, m._v1._mv);
		this->_v2._mv = _mm_add_ps(this->_v2._mv, m._v2._mv);
		this->_v3._mv = _mm_add_ps(this->_v3._mv, m._v3._mv);

		this->SetHint(a, b);

		return *this;
	}

	Mat4& Mat4::operator-=(const Mat4& m)
	{
		Hint a = this->GetHint();
		Hint b = m.GetHint();

		this->_v0._mv = _mm_sub_ps(this->_v0._mv, m._v0._mv);
		this->_v1._mv = _mm_sub_ps(this->_v1._mv, m._v1._mv);
		this->_v2._mv = _mm_sub_ps(this->_v2._mv, m._v2._mv);
		this->_v3._mv = _mm_sub_ps(this->_v3._mv, m._v3._mv);

		this->SetHint(a, b);

		return *this;
	}

	Mat4& Mat4::operator/=(float rhs)
	{
		Hint a = this->GetHint();

		Vec4 inverse;
		inverse._mv = _mm_set1_ps(1.0f / rhs);

		this->_v0._mv = _mm_mul_ps(this->_v0._mv, inverse._mv);
		this->_v1._mv = _mm_mul_ps(this->_v1._mv, inverse._mv);
		this->_v2._mv = _mm_mul_ps(this->_v2._mv, inverse._mv);
		this->_v3._mv = _mm_mul_ps(this->_v3._mv, inverse._mv);

		this->SetHint(a);

		return *this;
	}

	Mat4& Mat4::operator*=(float rhs)
	{
		Hint a = this->GetHint();

		Vec4 fPacked;
		fPacked._mv = _mm_set1_ps(rhs);

		this->_v0._mv = _mm_mul_ps(this->_v0._mv, fPacked._mv);
		this->_v1._mv = _mm_mul_ps(this->_v1._mv, fPacked._mv);
		this->_v2._mv = _mm_mul_ps(this->_v2._mv, fPacked._mv);
		this->_v3._mv = _mm_mul_ps(this->_v3._mv, fPacked._mv);

		this->SetHint(a);

		return *this;
	}

	Mat4 operator*(float f, const Mat4& m)
	{
		Mat4::Hint a = m.GetHint();

		Mat4 mRet;

		Vec4 fPacked;
		fPacked._mv = _mm_set1_ps(f);

		mRet._v0._mv = _mm_mul_ps(fPacked._mv, m._v0._mv);
		mRet._v1._mv = _mm_mul_ps(fPacked._mv, m._v1._mv);
		mRet._v2._mv = _mm_mul_ps(fPacked._mv, m._v2._mv);
		mRet._v3._mv = _mm_mul_ps(fPacked._mv, m._v3._mv);

		mRet.SetHint(a);

		return mRet;
	}

	Mat4 operator*(const Mat4& m, float f)
	{
		Mat4::Hint a = m.GetHint();

		Mat4 mRet;
		Vec4 fPacked;

		fPacked._mv = _mm_set1_ps(f);

		mRet._v0._mv = _mm_mul_ps(m._v0._mv, fPacked._mv);
		mRet._v1._mv = _mm_mul_ps(m._v1._mv, fPacked._mv);
		mRet._v2._mv = _mm_mul_ps(m._v2._mv, fPacked._mv);
		mRet._v3._mv = _mm_mul_ps(m._v3._mv, fPacked._mv);

		mRet.SetHint(a);

		return mRet;
	}


	Mat4 operator*(const Mat4& m, const Quat& q)
	{
		return m * Mat4(q);
	}

	Mat4& Mat4::operator*=(const Quat& q)
	{
		*this *= Mat4(q);

		return *this;
	}

	Mat4 Mat4::operator*(const Quat& q)
	{
		return (*this * Mat4(q));
	}

	Mat4& Mat4::operator*=(const Mat4& m)
	{
		Hint a = this->GetHint();
		Hint b = m.GetHint();

		Mat4 mTemp;

		__m128 row1 = _mm_load_ps(&m._m0);
		__m128 row2 = _mm_load_ps(&m._m4);
		__m128 row3 = _mm_load_ps(&m._m8);
		__m128 row4 = _mm_load_ps(&m._m12);

		const float* pF = &this->_m0;
		float* pRetF = &mTemp._m0;

		for (int i = 0; i < 4; i++)
		{
			__m128 x = _mm_set1_ps(pF[4 * i + 0]);
			__m128 y = _mm_set1_ps(pF[4 * i + 1]);
			__m128 z = _mm_set1_ps(pF[4 * i + 2]);
			__m128 w = _mm_set1_ps(pF[4 * i + 3]);
			__m128 row = _mm_add_ps(
				_mm_add_ps(_mm_mul_ps(x, row1), _mm_mul_ps(y, row2)),
				_mm_add_ps(_mm_mul_ps(z, row3), _mm_mul_ps(w, row4)));

			_mm_store_ps(&pRetF[4 * i], row);
		}

		*this = mTemp;

		this->SetHint(a, b);

		return *this;
	}

	bool Mat4::operator ==(const Mat4& m) const
	{
		return (this->_v0 == m._v0 &&
			this->_v1 == m._v1 &&
			this->_v2 == m._v2 &&
			this->_v3 == m._v3
			);
	}

	Mat4 Mat4::operator+() const
	{
		Mat4 m(*this);

		return m;
	}

	Mat4 Mat4::operator-() const
	{
		Mat4 m;

		Vec4 v;
		_mm_store_ps(&v._vx, _mm_setzero_ps());
		m._v0._mv = _mm_sub_ps(v._mv, this->_v0._mv);
		m._v1._mv = _mm_sub_ps(v._mv, this->_v1._mv);
		m._v2._mv = _mm_sub_ps(v._mv, this->_v2._mv);
		m._v3._mv = _mm_sub_ps(v._mv, this->_v3._mv);

		return m;
	}


	void Mat4::Print(const char* pName) const
	{
		Debug::Print(pName, *this);
	}

	void Mat4::SetScaleHint()
	{
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Hint::Scale;
	}

	void Mat4::SetRotHint()
	{
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Hint::Rot;
	}

	void Mat4::SetTransHint()
	{
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Hint::Trans;
	}

	void Mat4::SetGeneralHint()
	{
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)Hint::GeneralizeMat4;
	}

	void Mat4::SetHint(Hint a)
	{
		uint32_t* p = (uint32_t*)&this->_m15;
		*p |= (uint32_t)a;
	}

	void Mat4::SetHint(Mat4::Hint a, Mat4::Hint b)
	{
		Hint h;

		if (a == Hint::GeneralizeMat4 || a == Hint::GeneralizedAffine || b == Hint::GeneralizeMat4 || b == Hint::GeneralizedAffine)
		{
			h = Hint::GeneralizeMat4;
		}
		else
		{
			h = (Hint)((uint32_t)a | (uint32_t)b);
		}

		uint32_t* p = (uint32_t*)&this->_m15;
		*p &= (uint32_t)0xFFFFFFF0;
		*p |= (uint32_t)h;
	}

	Mat4::Hint Mat4::GetHint() const
	{
		Hint h;
		uint32_t* p = (uint32_t*)&this->_m15;

		h = (Hint)(*p & (uint32_t)0xFF);

		return h;
	}

	bool Mat4::privHasHint() const
	{
		const uint32_t* p = (const uint32_t*)&this->_m15;
		return ((*p & 0xFFFFFFF0) == 0x3f800000);
	}

}

// ---  End of File ---
