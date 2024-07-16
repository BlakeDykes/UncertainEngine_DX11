#pragma once

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Constants.h"
#include "Vec4Proxy.h"
#include "Enum.h"

#include <complex.h>
#include <complex>
#undef complex

// This file assumes Framework.h is included in executable for Align16

namespace Uncertain
{
	class Mat4;
	class Mat3;
	class Vec3;
	class Quat;

	class Vec4 final : public Align16
	{
	public:

		Vec4();
		Vec4(float x, float y, float z, float w = 1.0f);
		explicit Vec4(const Vec3& v, const float w = 1.0f);
		Vec4(const Vec4& v);
		Vec4& operator =(const Vec4& v);
		~Vec4() = default;

		const float& operator[](const x_enum) const { return _vx; }
		const float& operator[](const y_enum) const { return _vy; }
		const float& operator[](const z_enum) const { return _vz; }
		const float& operator[](const w_enum) const { return _vw; }
		const float& x() const { return _vx; }
		const float& y() const { return _vy; }
		const float& z() const { return _vz; }
		const float& w() const { return _vw; }

		// Setters
		void x(const float v) { this->_vx = v; }
		void y(const float v) { this->_vy = v; }
		void z(const float v) { this->_vz = v; }
		void w(const float v) { this->_vw = v; }
		void r0(const float f) { this->_r0 = f; }
		void i0(const float f) { this->_i0 = f; }
		void r1(const float f) { this->_r1 = f; }
		void i1(const float f) { this->_i1 = f; }

		float& operator[](const x_enum) { return _vx; }
		float& operator[](const y_enum) { return _vy; }
		float& operator[](const z_enum) { return _vz; }
		float& operator[](const w_enum) { return _vw; }
		float& operator[](const real0_enum) { return _vx; }
		float& operator[](const imag0_enum) { return _vy; }
		float& operator[](const real1_enum) { return _vz; }
		float& operator[](const imag1_enum) { return _vw; }


		Vec4 operator+(const Vec4& v) const;
		Vec4& operator+=(const Vec4& v);
		Vec4 operator-(const Vec4& v) const;
		Vec4& operator-=(const Vec4& v);
		Vec4 operator*(const Vec4& v) const;
		Vec4& operator*=(const Vec4& v);
		Vec4 operator/(const Vec4& v) const;
		Vec4& operator/=(const Vec4& v);
		bool operator==(const Vec4& v) const;
		bool operator!=(const Vec4& v) const;
		bool operator==(const Vec3& v) const;
		Vec4 operator+() const;
		Vec4 operator-() const;

		// scale operators
		Vec4 operator+(const float f) const;
		Vec4 operator-(const float f) const;
		Vec4 operator*(const float f) const;
		Vec4 operator/(const float f) const;
		Vec4& operator*=(const float f);
		friend Vec4 operator*(float f, const Vec4& v);

		// Mat4
		Vec4 operator*(const Mat4& m) const;
		Vec4& operator*=(const Mat4& m);
		
		// Mat3
		Vec4& operator*=(const Mat3& m);

		// Quat
		Vec4 operator*(const Quat& q);
		Vec4& operator*=(const Quat& q);
		Vec4& operator=(const Quat& q);

		// Vec3
		Vec4& operator=(const Vec3& v);

		friend Mat4 operator*(const float f, const Mat4& m);
		friend Mat4 operator*(const Mat4& m, float f);
		friend Mat3 operator*(float f, const Mat3& m);
		friend Mat3 operator*(const Mat3& m, float f);
		friend Vec4 operator*(const Vec4& v, const Mat3& m);
		//friend Vec4 operator*(const Vec3& v, const Mat4& m);

		// Vec4 functions
		Vec4 cross(const Vec4& v) const;
		float dot(const Vec4& v) const;
		void norm();
		Vec4 getNorm() const;
		float getAngle(const Vec4& v) const;
		Vec4& negateXYZ();
		const Vec4Proxy len() const;

		void DFT_2Point_Real(Vec4& vOut);
		void DFT_ComplexMult(const Vec4& v1, const Vec4& v2);

		void clear();

		// set
		void set(const Vec3& v, const float w = 1.0f);
		void set(std::complex<float> c);
		Vec4& set(const float x, const float y, const float z, const float w = 1.0f);
		Vec4& set(const Vec4& v);

		// comparison
		bool isEqual(const Vec4& v, const float tolerance = MATH_TOLERANCE) const;
		bool isZero(const float tolerance = MATH_TOLERANCE) const;

		// for printing
		void Print(const char* pName) const;

	private:

		friend Mat4;
		friend Mat3;
		friend Vec3;
		friend Quat;

		union
		{
			__m128	_mv;

			// anonymous struct
			struct
			{
				float _vx;
				float _vy;
				float _vz;
				float _vw;
			};

			struct
			{
				float _r0;
				float _i0;
				float _r1;
				float _i1;
			};
		};
	};
}

//--- End of File ---
