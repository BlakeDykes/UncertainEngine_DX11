#pragma once

namespace Uncertain
{

	struct Vec4Proxy
	{
	public:
		Vec4Proxy(const float _x, const float _y, const float _z, const float _w);

		bool operator==(const Vec4Proxy& mP) const;
		bool operator!=(const Vec4Proxy& mP) const;
		bool operator<(const Vec4Proxy& mP) const;
		bool operator<=(const Vec4Proxy& mP) const;
		bool operator>(const Vec4Proxy& mP) const;
		bool operator>=(const Vec4Proxy& mP) const;

		operator float() const;

	private:
		float x;
		float y;
		float z;
		float w;
	};

}

//--- End of File ---
