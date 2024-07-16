#pragma once

namespace Uncertain
{

	struct QuatProxy
	{
	public:
		QuatProxy(const float _x, const float _y, const float _z, const float _w);

		bool operator==(const QuatProxy& mP) const;
		bool operator!=(const QuatProxy& mP) const;
		bool operator<(const QuatProxy& mP) const;
		bool operator<=(const QuatProxy& mP) const;
		bool operator>(const QuatProxy& mP) const;
		bool operator>=(const QuatProxy& mP) const;

		operator float() const;

	private:
		float x;
		float y;
		float z;
		float w;
	};

}

//--- End of File ---
