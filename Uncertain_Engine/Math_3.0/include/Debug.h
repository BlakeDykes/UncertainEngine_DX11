#pragma once

namespace Uncertain
{
	// Forward declarations:
	class Vec4; 
	class Vec3;
	class Mat4;
	class Mat3;
	class Quat;

	// ----------------------------------------------------------------------
	// Output Print for Vec4 and Mat4
	// ----------------------------------------------------------------------
	class Debug
	{
	public:
		static void Print(const char* pName, const Vec4 &r);
		static void Print(const char *pName, const Vec3 &r);
		static void Print(const char* pName, const Mat4 &r);
		static void Print(const char *pName, const Mat3 &r);
		static void Print(const char *pName, const Quat &r);
	};

}

//--- End of File ---
