#pragma once

#include "Vec4.h"
#include "Debug.h"
#include "Mat4.h"
#include "Vec3.h"
#include "Enum.h"

namespace Uncertain
{

	class Mat3 final : public Align16
	{
	public:

		enum class Special
		{
			Zero,
			Identity
		};

		enum class Row
		{
			i0,
			i1,
			i2
		};

	public:

		// Do your magic here
		static Mat3 IDENTITY_MAT3;
		static Mat3 ZERO_MAT3;

		Mat3();
		Mat3(Vec3& a, Vec3& b, Vec3& c);
		Mat3(float m0, float m1, float m2,
			float m4, float m5, float m6,
			float m8, float m9, float m10);
		Mat3(const Mat3& m);
		Mat3& operator =(Mat3 m);
		Mat3(const Mat4& m);
		Mat3& operator=(Mat4 m);
		~Mat3() = default;

		explicit Mat3(const Special type);
		void clear();
		Vec3 get(Mat3::Row row);
		const float det() const;
		Mat3 getT() const;
		Mat3& T();
		Mat3& inv();
		Mat3 getInv() const;
		Mat3& adj();
		Mat3 GetAdj() const;

		// Bracket Getters
		const float& operator[](m0_enum) const { return _m0; }
		const float& operator[](m1_enum) const { return _m1; }
		const float& operator[](m2_enum) const { return _m2; }
		const float& operator[](m4_enum) const { return _m4; }
		const float& operator[](m5_enum) const { return _m5; }
		const float& operator[](m6_enum) const { return _m6; }
		const float& operator[](m8_enum) const { return _m8; }
		const float& operator[](m9_enum) const { return _m9; }
		const float& operator[](m10_enum) const { return _m10; }

		// Getters
		const float& m0() const { return _m0; }
		const float& m1() const { return _m1; }
		const float& m2() const { return _m2; }
		const float& m4() const { return _m4; }
		const float& m5() const { return _m5; }
		const float& m6() const { return _m6; }
		const float& m8() const { return _m8; }
		const float& m9() const { return _m9; }
		const float& m10() const { return _m10; }

		static const Mat3& getIdentity();
		static const Mat3& getZero();

		// Setters
		void m0(const float v) { this->_m0 = v; }
		void m1(const float v) { this->_m1 = v; }
		void m2(const float v) { this->_m2 = v; }
		void m4(const float v) { this->_m4 = v; }
		void m5(const float v) { this->_m5 = v; }
		void m6(const float v) { this->_m6 = v; }
		void m8(const float v) { this->_m8 = v; }
		void m9(const float v) { this->_m9 = v; }
		void m10(const float v) { this->_m10 = v; }
		float& operator[](m0_enum) { return _m0; }
		float& operator[](m1_enum) { return _m1; }
		float& operator[](m2_enum) { return _m2; }
		float& operator[](m4_enum) { return _m4; }
		float& operator[](m5_enum) { return _m5; }
		float& operator[](m6_enum) { return _m6; }
		float& operator[](m8_enum) { return _m8; }
		float& operator[](m9_enum) { return _m9; }
		float& operator[](m10_enum) { return _m10; }


		Mat3& set(Mat3::Special type);
		Mat3& set(Mat3::Row row, const Vec3& v);
		Mat3& set(const Vec3& a, const Vec3& b, const Vec3& c);


		// Comparison
		bool isEqual(const Mat3& m);
		bool isIdentity(const float tolerance) const;
		bool isIdentity() const;
		bool isZero(const float tolerance) const;
		bool isRotation() const;

		// Scale operators
		Mat3 operator+(const Mat3& m) const;
		Mat3 operator-(const Mat3& m) const;
		Mat3 operator*(const Mat3& m) const;
		Mat3& operator+=(const Mat3& m);
		Mat3& operator-=(const Mat3& m);
		bool operator ==(const Mat3& m) const;
		Mat3& operator*=(const Mat3& m);
		Mat3 operator+() const;
		Mat3 operator-() const;

		Mat3& operator/=(float rhs);
		Mat3& operator*=(float rhs);

		friend Mat3 operator*(float f, const Mat3& m);
		friend Mat3 operator*(const Mat3& m, float f);
		friend Vec4 operator*(const Vec4& v, const Mat3& m);

		// For printing
		void Print(const char* pName) const;


	private:

		friend Mat4;
		friend Vec3;
		friend Vec4;

		union
		{
			/*     | m0  m1  m2   0 | */
			/* m = | m4  m5  m6   0 | */
			/*     | m8  m9  m10  0 | */
			/*     | 0   0   0    1 | */

			struct
			{
				Vec4 _rows[4];
			};


			struct
			{
				Vec4 _v0;
				Vec4 _v1;
				Vec4 _v2;
				Vec4 _v3;
			};

			struct
			{
				float _m0;
				float _m1;
				float _m2;
				float _m3;
				float _m4;
				float _m5;
				float _m6;
				float _m7;
				float _m8;
				float _m9;
				float _m10;
				float _m11;
				float _m12;
				float _m13;
				float _m14;
				float _m15;
			};
		};
	};
}

//--- End of File ---
