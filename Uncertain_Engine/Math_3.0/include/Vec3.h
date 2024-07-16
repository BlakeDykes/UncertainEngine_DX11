#pragma once

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Constants.h"
#include "Vec3Proxy.h"
#include "Enum.h"

// This file assumes Framework.h is included in executable for Align16

namespace Uncertain
{
	// forward declare
	class Mat3;
	class Vec4;
	class Mat4;
	class Quat;

	class Vec3 final : public Align16
	{
	public:

		Vec3();
		Vec3(float x, float y, float z);
		explicit Vec3(const Vec4& v);
		Vec3(const Vec3&);
		Vec3& operator =(const Vec3& v);
		Vec3& operator =(const Vec4& v);
		~Vec3() = default;

		// Accessors
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
		float& operator[](const x_enum) { return _vx; }
		float& operator[](const y_enum) { return _vy; }
		float& operator[](const z_enum) { return _vz; }
		float& operator[](const w_enum) { return _vw; }


		Vec3 operator+(const Vec3& v) const;
		Vec3 operator+=(const Vec3& v);
		Vec3 operator-(const Vec3& v) const;
		Vec3 operator-=(const Vec3& v);
		Vec3 operator*(const Vec3& v) const;
		Vec3 operator*=(const Vec3& v);
		Vec3 operator/(const Vec3& v) const;
		Vec3 operator/=(const Vec3& v);
		bool operator==(const Vec3& v) const;
		bool operator==(const Vec4& v) const;
		Vec3 operator+() const;
		Vec3 operator-() const;

		// scale operators
		Vec3 operator+(const float f) const;
		Vec3 operator-(const float f) const;
		Vec3 operator*(const float f) const;
		Vec3 operator/(const float f) const;
		Vec3& operator*=(const float f);
		friend Vec3 operator*(float f, const Vec3& v);
		friend Vec4 operator*(const Vec4& v, const Mat3& m);
		friend Vec4 operator*(const Vec3& v, const Mat4& m);

		Vec3 operator*(const Mat3& m) const;
		Vec3& operator*=(const Mat3& m);

		Vec3 operator*(const Quat& q);
		Vec3& operator*=(const Quat& q);
		Vec3& operator=(const Quat& q);

		Vec3 cross(const Vec3& v) const;
		float dot(const Vec3& v) const;
		void norm();
		Vec3 getNorm() const;
		const Vec3Proxy len() const;
		float getAngle(const Vec3& v) const;

		void clear();

		// set
		void set(const Vec3& v);
		void set(const Vec4& v);
		Vec3& set(const float x, const float y, const float z);

		// comparison
		bool isEqual(const Vec3& v, const float tolerance = MATH_TOLERANCE) const;
		bool isZero(const float tolerance = MATH_TOLERANCE) const;


		// for printing
		void Print(const char* pName) const;

	private:

		friend Mat3;
		friend Mat4;
		friend Vec4;
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
				float _vw;  // Not used in this class
			};
		};
	};
}

//--- End of File ---
