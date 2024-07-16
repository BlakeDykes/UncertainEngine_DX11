#pragma once

namespace Uncertain
{
	struct Vec3Proxy
	{
	public:
		Vec3Proxy(const float _x, const float _y, const float _z);

		bool operator==(const Vec3Proxy& mP) const;
		bool operator!=(const Vec3Proxy& mP) const;
		bool operator<(const Vec3Proxy& mP) const;
		bool operator<=(const Vec3Proxy& mP) const;
		bool operator>(const Vec3Proxy& mP) const;
		bool operator>=(const Vec3Proxy& mP) const;

		operator float() const;

	private:
		float x;
		float y;
		float z;
	};

}

//--- End of File ---
