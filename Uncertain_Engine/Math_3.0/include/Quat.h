#pragma once

#include "Mat4.h"
#include "Debug.h"
#include "QuatProxy.h"
#include "Enum.h"

namespace Uncertain
{
	class Vec3;

	class Quat final : public Align16
	{
	public:
		enum class Rot1
		{
			X,
			Y,
			Z
		};

		enum class Special
		{
			Zero,
			Identity
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

		enum class Rot3
		{
			XYZ
			// future combos... here
		};

	public:
		Quat();
		Quat(const Quat& q);
		Quat& operator=(const Quat& q);
		~Quat() = default;

		Quat(float x, float y, float z, float w);
		explicit Quat(const Vec4& v);
		explicit Quat(const Vec3& v, float w);
		explicit Quat(const Mat4& m);

		Quat(Quat::Rot1 rot, float angle);
		Quat(const Quat::Rot3, const float x, const float y, const float z);
		Quat(const Quat::Rot, const Vec3& axis, const float angle);
		Quat(const Quat::Special type);
		Quat(const Quat::Orient type, const Vec3& dof, const Vec3& up);


		Quat& norm();
		Quat getNorm() const;
		Vec3 cross(const Quat& q) const;
		float dot(const Quat& q) const;
		Quat getT() const;
		Quat& T();
		const QuatProxy mag() const;
		float magSquared() const;
		float invMag() const;
		Quat getInv() const;
		Quat& inv();
		Quat& conj();
		Quat getConj() const;
		Quat getNeg() const;
		void getVec3(Vec3& vOut);
		float getAngle() const;
		void getAxis(Vec3& vOut);
		void Lqcvq(const Vec3& vIn, Vec3& vOut) const;
		void Lqvqc(const Vec3& vIn, Vec3& vOut) const;

		const bool isEqual(const Quat& q, const float epsilon = MATH_TOLERANCE) const;
		const bool isEquivalent(const Quat& q, const float epsilon = MATH_TOLERANCE) const;
		const bool isNegEqual(const Quat& q, const float epsilon = MATH_TOLERANCE) const;
		const bool isConjugateEqual(const Quat& q, const float epsilon = MATH_TOLERANCE) const;
		const bool isIdentity(const float epsilon = MATH_TOLERANCE) const;
		const bool isNormalized(const float epsilon = MATH_TOLERANCE) const;
		const bool isZero(const float epsilon = MATH_TOLERANCE) const;

		void clear();

		Quat& set(const float x, const float y, const float z, const float real);
		Quat& set(Vec4& v);
		Quat& set(Vec3& v, float real);
		Quat& set(Mat4& m);
		Quat& set(Quat::Rot1 rot, float angle);
		Quat& set(const Quat::Rot3, const float x, const float y, const float z);
		Quat& set(const Quat::Rot, const Vec3& axis, const float angle);
		Quat& set(const Quat::Special type);
		Quat& set(const Quat::Orient type, const Vec3& dof, const Vec3& up);
		Quat& setVec3(const Vec3& v);

		// Getters
		const float& operator[](const x_enum) const { return _qx; }
		const float& operator[](const y_enum) const { return _qy; }
		const float& operator[](const z_enum) const { return _qz; }
		const float& operator[](const w_enum) const { return _qw; }
		const float& qx() const { return _qx; }
		const float& qy() const { return _qy; }
		const float& qz() const { return _qz; }
		const float& real() const { return _qw; }

		// Setters
		void qx(const float v) { this->_qx = v; }
		void qy(const float v) { this->_qy = v; }
		void qz(const float v) { this->_qz = v; }
		void real(const float v) { this->_qw = v; }
		float& operator[](const x_enum) { return _qx; }
		float& operator[](const y_enum) { return _qy; }
		float& operator[](const z_enum) { return _qz; }
		float& operator[](const w_enum) { return _qw; }
		
		// Operators
		Quat operator+(const Quat& q) const;
		Quat& operator+=(const Quat& q);
		Quat operator-(const Quat& q) const;
		Quat& operator-=(const Quat& q);
		Quat operator*(const Quat& q) const;
		Quat& operator*=(const Quat& q);
		Quat operator/(const Quat& q) const;
		Quat& operator/=(const Quat& q);
		bool operator==(const Quat& q) const;
		bool operator==(const Vec4& v) const;
		Quat operator+() const;
		Quat operator-() const;

		Quat operator+(const float& f) const;
		Quat& operator+=(const float& f);
		Quat operator-(const float& f) const;
		Quat& operator-=(const float& f);
		Quat operator*(const float& f) const;
		Quat& operator*=(const float& f);
		Quat operator/(const float& f) const;
		Quat& operator/=(const float& f);
		friend Quat operator+(float f, const Quat& q);
		friend Quat operator-(float f, const Quat& q);
		friend Quat operator*(float f, const Quat& q);
		friend Quat operator/(float f, const Quat& q);

		Quat operator*(const Mat4& m) const;
		Quat& operator*=(const Mat4& m);
		Quat& operator=(const Mat4& m);


	private:
		friend Vec4;
		friend Mat4;
		friend Vec3;

		// anonymous union
		union
		{
			__m128	_mq;

			// anonymous struct
			struct
			{
				float _qx;
				float _qy;
				float _qz;
				float _qw;
			};

			struct
			{
				Vec3  _qV3;
			};

			struct
			{
				Vec4 _qV4;
			};
		};

	};
}

//--- End of File ---
