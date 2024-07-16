#pragma once

#include "Vec4.h"
#include "Vec3.h"
#include "Debug.h"
#include "Enum.h"

namespace Uncertain
{
	class Quat;

	class Mat4 final : public Align16
	{
	public:

		enum class Special
		{
			Zero,
			Identity
		};

		enum class Rot1
		{
			X,
			Y,
			Z
		};

		enum class Trans
		{
			XYZ
		};

		enum class Scale
		{
			XYZ
		};

		enum class Rot
		{
			AxisAngle,
		};

		enum class Orient
		{
			LocalToWorld,
			WorldToLocal
		};

		enum class Row
		{
			i0,
			i1,
			i2,
			i3
		};

		enum class Rot3
		{
			XYZ
			// future combos... here
		};

		enum class Hint
		{
			GeneralizeMat4 = 0x0,     // generalize 4x4 inverse  (no flags)
			Rot = 0x1,                // Mat4 inverse is M.T()
			Trans = 0x2,              // Mat4 inverse is M(-trans)
			RotTrans = 0x3,           // rot and trans  A_inv is A.T() 
			Scale = 0x4,              // Mat4 invers is M(1/scale)
			RotScale = 0x5,           // Rot and Scale no translate
			TransScale = 0x6,         // Trans and Scale no Translate
			GeneralizedAffine = 0x7,  // generalize Affine
		};

		struct Mat4Proxy
		{
			float &x;

			inline Mat4Proxy(float& _x)
				:x(_x)
			{
			}

			inline Mat4Proxy operator=(float _x)
			{
				x = _x;
				uint32_t* p = (uint32_t*)&this->x;
				*p |= (uint32_t)Mat4::Hint::GeneralizeMat4;

				return *this;
			}

			inline operator const float()
			{
				return this->x;
			}
		};

	public:

		// Do your magic here
		static Mat4 IDENTITY_MAT4;
		static Mat4 ZERO_MAT4;

		Mat4();
		Mat4(Vec4 a, Vec4 b, Vec4 c, Vec4 d);
		Mat4(float m0, float m1, float m2, float m3,
			float m4, float m5, float m6, float m7,
			float m8, float m9, float m10, float m11,
			float m12, float m13, float m14, float m15);
		Mat4(Mat4::Orient orient, const Vec3 dof, const Vec3 up);
		Mat4(Mat4::Rot axis, const Vec3& v, const float radians);
		Mat4(Mat4::Rot1 rot, float angle);
		Mat4(Mat4::Rot3 rot, float xAngle, float yAngle, float zAngle);
		Mat4(Mat4::Trans dir, float xDist = 0.0f, float yDist = 0.0f, float zDist = 0.0f);
		Mat4(Mat4::Scale scale, float xScale = 1.0f, float yScale = 1.0f, float zScale = 1.0f);
		Mat4(Mat4::Scale scale, const Vec3& v);
		Mat4(Mat4::Special type);
		Mat4(Mat4::Trans type, const Vec3& v);
		explicit Mat4(const Quat& q);

		Mat4(const Mat4& m);
		Mat4& operator =(const Mat4& m);
		Mat4& operator =(const Mat3& m);
		~Mat4() = default;

		void clear();
		Vec4& get(Mat4::Row row);
		const float det() const;
		Mat4 getT() const;
		void T();
		void inv();
		Mat4 getInv() const;
		Mat4& adj();
		Mat4 GetAdj() const;

		// Bracket Getters
		const float& operator[](m0_enum) const { return _m0; }
		const float& operator[](m1_enum) const { return _m1; }
		const float& operator[](m2_enum) const { return _m2; }
		const float& operator[](m3_enum) const { return _m3; }
		const float& operator[](m4_enum) const { return _m4; }
		const float& operator[](m5_enum) const { return _m5; }
		const float& operator[](m6_enum) const { return _m6; }
		const float& operator[](m7_enum) const { return _m7; }
		const float& operator[](m8_enum) const { return _m8; }
		const float& operator[](m9_enum) const { return _m9; }
		const float& operator[](m10_enum) const { return _m10; }
		const float& operator[](m11_enum) const { return _m11; }
		const float& operator[](m12_enum) const { return _m12; }
		const float& operator[](m13_enum) const { return _m13; }
		const float& operator[](m14_enum) const { return _m14; }
		const float& operator[](m15_enum) const { return _m15; }

		// Getters
		const float& m0() const { return _m0; }
		const float& m1() const { return _m1; }
		const float& m2() const { return _m2; }
		const float& m3() const { return _m3; }
		const float& m4() const { return _m4; }
		const float& m5() const { return _m5; }
		const float& m6() const { return _m6; }
		const float& m7() const { return _m7; }
		const float& m8() const { return _m8; }
		const float& m9() const { return _m9; }
		const float& m10() const { return _m10; }
		const float& m11() const { return _m11; }
		const float& m12() const { return _m12; }
		const float& m13() const { return _m13; }
		const float& m14() const { return _m14; }
		const float& m15() const { return _m15; }


		static const Mat4& getIdentity();
		static const Mat4& getZero();

		// Setters
		void m0(const float v);
		void m1(const float v);
		void m2(const float v);
		void m3(const float v);
		void m4(const float v);
		void m5(const float v);
		void m6(const float v);
		void m7(const float v);
		void m8(const float v);
		void m9(const float v);
		void m10(const float v);
		void m11(const float v);
		void m12(const float v);
		void m13(const float v);
		void m14(const float v);
		void m15(const float v);
		Mat4Proxy operator[](m0_enum) { return Mat4Proxy(_m0); }
		Mat4Proxy operator[](m1_enum) { return Mat4Proxy(_m1); }
		Mat4Proxy operator[](m2_enum) { return Mat4Proxy(_m2); }
		Mat4Proxy operator[](m3_enum) { return Mat4Proxy(_m3); }
		Mat4Proxy operator[](m4_enum) { return Mat4Proxy(_m4); }
		Mat4Proxy operator[](m5_enum) { return Mat4Proxy(_m5); }
		Mat4Proxy operator[](m6_enum) { return Mat4Proxy(_m6); }
		Mat4Proxy operator[](m7_enum) { return Mat4Proxy(_m7); }
		Mat4Proxy operator[](m8_enum) { return Mat4Proxy(_m8); }
		Mat4Proxy operator[](m9_enum) { return Mat4Proxy(_m9); }
		Mat4Proxy operator[](m10_enum) { return Mat4Proxy(_m10); }
		Mat4Proxy operator[](m11_enum) { return Mat4Proxy(_m11); }
		Mat4Proxy operator[](m12_enum) { return Mat4Proxy(_m12); }
		Mat4Proxy operator[](m13_enum) { return Mat4Proxy(_m13); }
		Mat4Proxy operator[](m14_enum) { return Mat4Proxy(_m14); }
		Mat4Proxy operator[](m15_enum) { return Mat4Proxy(_m15); }

		Mat4& set(Mat4::Special type);
		Mat4& set(Mat4::Trans type, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		Mat4& set(Mat4::Trans type, const Vec3& v);
		Mat4& set(Mat4::Scale type, float x = 0.0f, float y = 0.0f, float z = 0.0f);
		Mat4& set(Mat4::Scale type, const Vec3& v);
		Mat4& set(Mat4::Rot1 axis, float radians);
		Mat4& set(Mat4::Rot3 axis, float xAngle, float yAngle, float zAngle);
		Mat4& set(Mat4::Rot axis, const Vec3& v, const float radians);
		Mat4& set(Mat4::Row row, const Vec3& v);
		Mat4& set(Mat4::Row row, const Vec4& v);
		Mat4& set(Mat4::Orient orient, const Vec3 dof, const Vec3 up);
		Mat4& set(const Vec3& a, const Vec3& b, const Vec3& c, const Vec3& d);
		Mat4& set(const Vec4& a, const Vec4& b, const Vec4& c, const Vec4& d);
		Mat4& set(const Quat& q);
		
		// Comparison
		//const bool isRotation(const float epsilon = MATH_TOLERANCE) const;
		bool isEqual(const Mat4& m);
		bool isIdentity(const float tolerance) const;
		bool isIdentity() const;
		bool isZero(const float tolerance) const;
		bool isRotation() const;

		// Scale operators
		//Mat4 operator * (const float s) const;
		//friend Mat4 operator *(const float scale, const Mat4 &A);
		//void operator *= (const float scale);
		Mat4 operator+(const Mat4& m) const;
		Mat4 operator-(const Mat4& m) const;
		Mat4 operator*(const Mat4& m) const;
		Mat4& operator+=(const Mat4& m);
		Mat4& operator-=(const Mat4& m);
		bool operator ==(const Mat4& m) const;
		Mat4& operator*=(const Mat4& m);
		Mat4 operator+() const;
		Mat4 operator-() const;

		Mat4& operator/=(float rhs);
		Mat4& operator*=(float rhs);

		friend Mat4 operator*(float f, const Mat4& m);
		friend Mat4 operator*(const Mat4& m, float f);
		friend Vec4 operator*(const Vec3& v, const Mat4& m);
		friend Mat4 operator*(const Mat4& m, const Quat& q);

		Mat4& operator*=(const Quat& q);
		Mat4 operator*(const Quat& q);


		// For printing
		void Print(const char* pName) const;

		// Hints
		void SetScaleHint();
		void SetRotHint();
		void SetTransHint();
		void SetGeneralHint();
		void SetHint(Hint a);
		void SetHint(Hint a, Hint b);
		Hint GetHint() const;
		bool privHasHint() const;

	private:

		friend Mat3;
		friend Vec3;
		friend Vec4;
		friend Quat;

		union
		{
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
