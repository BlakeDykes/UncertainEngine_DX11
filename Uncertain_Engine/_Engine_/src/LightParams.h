#pragma once

#include "Mat4.h"
#include "Vec4.h"
#include "d3dUtil.h"
#include "DXApp.h"

static const float TURN_OFF_VALUE = -75.0f;
static const float OFF_CHECK_VALUE = -50.0f;
static const Vec4 PHONG_ZERO_VECT = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
static const Vec4 PHONG_DEFAULT_AMBIENT = Colors::Magenta;
static const Vec4 PHONG_DEFAULT_DIFFUSE = Colors::Magenta;
static const Vec4 PHONG_DEFAULT_SPECULAR = Vec4(1.0f, 1.0f, 1.0f, 20.0f);
static const Vec4 PHONG_DEFAULT_DIRECTION = Vec4(0.0, -1.0f, 1.0f).getNorm();
static const Vec4 PHONG_DEFAULT_POSITION = Vec4(0.0f, 5.0f, -5.0f);
static const float PHONG_DEFAULT_RANGE = 100;
static const Vec4 PHONG_DEFAULT_ATTENUATION = Vec4(0.2f, 0.2f, 0.2f);
static const float PHONG_DEFAULT_SPOT_EXP = 5.0f;

struct PhongADS_Directional
{
	Vec4 Ambient;  // <--- w hold offset
	Vec4 Diffuse;  
	Vec4 Specular; // <--- w holds specular exponent
	Vec4 Direction;

	PhongADS_Directional(size_t offset)
		: Ambient(PHONG_ZERO_VECT.x(), PHONG_ZERO_VECT.y(), PHONG_ZERO_VECT.z(), (float)offset),
		Diffuse(PHONG_ZERO_VECT),
		Specular(PHONG_ZERO_VECT),
		Direction(PHONG_ZERO_VECT)
	{ }
	PhongADS_Directional() = delete;
	PhongADS_Directional(const PhongADS_Directional& l);
	PhongADS_Directional& operator=(const PhongADS_Directional& l);
	~PhongADS_Directional() = default;


	void Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& direction);
	size_t GetOffset() const { return (int)(Ambient.w() + 0.5f); }
	void Clean();

	void Reflect(const Mat4& m);
	
	void TurnOff(float& specCacheOut) { specCacheOut = Specular.w(); Specular[w] = TURN_OFF_VALUE; }
	void TurnOn(float specVal) { assert(specVal >= OFF_CHECK_VALUE); Specular[w] = specVal; }

	void SetAmbient(const Vec4& v) { Ambient.set(v.x(), v.y(), v.z(), Ambient.w()); }
	void SetDiffuse(const Vec4& v) { Diffuse.set(v); }
	void SetSpecular(const Vec4& v) { Specular.set(v); }
	void SetSpecularExponent(float f) { Specular[w] = f; }
	void SetDirection(const Vec4& v) { Direction.set(v); }

	Vec4 GetDirection() const { return Direction; }
	float GetSpecularExponent() const { return Specular.w(); }

#ifdef LIGHT_DEBUG	
	void DEBUG_Print(const char* const id);
#endif
};

struct PhongADS_Point
{
	Vec4 Ambient;  // <--- w hold offset
	Vec4 Diffuse;  // <--- w holds range
	Vec4 Specular; // <--- w holds specular exponent
	Vec4 Position; 
	Vec4 Attenuation; 

	PhongADS_Point(size_t offset)
		: Ambient(PHONG_ZERO_VECT.x(), PHONG_ZERO_VECT.y(), PHONG_ZERO_VECT.z(), (float)offset),
		Diffuse(PHONG_ZERO_VECT),
		Specular(PHONG_ZERO_VECT),
		Position(PHONG_ZERO_VECT),
		Attenuation(PHONG_ZERO_VECT)
	{}
	PhongADS_Point() = delete;
	PhongADS_Point(const PhongADS_Point& l);
	PhongADS_Point& operator=(const PhongADS_Point& l);
	~PhongADS_Point() = default;


	void Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular,
		const Vec4& position, const Vec4& attenuation, float range);
	size_t GetOffset() const { return (int)(Ambient.w() + 0.5f); }
	void Clean();

	void Reflect(const Mat4& m);
	void Translate(const Vec4& v) { Position += v; }
	void Translate(const Vec3& v) { Position += Vec4(v, 0.0f); }
	void ReceiveParentWorldMat(Mat4& m) { Position += m.get(Mat4::Row::i3); }

	void TurnOff(float& specCacheOut) { specCacheOut = Specular.w(); Specular[w] = TURN_OFF_VALUE; }
	void TurnOn(float specVal) { assert(specVal >= OFF_CHECK_VALUE); Specular[w] = specVal; }

	void SetAmbient(const Vec4& v) { Ambient.set(v.x(), v.y(), v.z(), Ambient.w()); }
	void SetDiffuse(const Vec4& v) { Diffuse.set(v.x(), v.y(), v.z(), Diffuse.w()); }
	void SetSpecular(const Vec4& v) { Specular.set(v); }
	void SetSpecularExponent(float f) { Specular[w] = f; }
	void SetPosition(const Vec4& v) { Position.set(v); }
	void SetAttenuation(const Vec4& v) { Attenuation.set(v); }
	void SetRange(float f) { Diffuse[w] = f; }

	Vec4 GetPosition() const { return Position; }
	float GetRange() const { return Diffuse.w(); }
	float GetSpecularExponent() const { return Specular.w(); }

#ifdef LIGHT_DEBUG
	void DEBUG_Print(const char* const id);
#endif
};

struct PhongADS_Spot
{
	Vec4 Ambient;   // <--- w holds offset
	Vec4 Diffuse;   // <--- w holds spot exponent
	Vec4 Specular;  // <--- w holds specular exponent
	Vec4 Position; 
	Vec4 Attenuation; 
	Vec4 Direction; // <--- w holds range

	PhongADS_Spot(size_t offset)
		: Ambient(PHONG_ZERO_VECT.x(), PHONG_ZERO_VECT.y(), PHONG_ZERO_VECT.z(), (float)offset),
		Diffuse(PHONG_ZERO_VECT),
		Specular(PHONG_ZERO_VECT),
		Position(PHONG_ZERO_VECT),
		Attenuation(PHONG_ZERO_VECT),
		Direction(PHONG_ZERO_VECT)
	{ }
	PhongADS_Spot() = delete;
	PhongADS_Spot(const PhongADS_Spot& l);
	PhongADS_Spot& operator=(const PhongADS_Spot& l);
	~PhongADS_Spot() = default;


	void Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular,
		const Vec4& position, const Vec4& attenuation, const Vec4& direction, float range, float spotExp);
	size_t GetOffset() const { return (int)(Ambient.w() + 0.5f); }
	void Clean();

	void TurnOff(float& specCacheOut) { specCacheOut = Specular.w(); Specular[w] = TURN_OFF_VALUE; }
	void TurnOn(float specVal) { assert(specVal >= OFF_CHECK_VALUE); Specular[w] = specVal; }

	void Reflect(const Mat4& m);
	void Translate(const Vec4& v) { Position += v; }
	void Translate(const Vec3& v) { Position += Vec4(v, 0.0f); }
	void ReceiveParentWorldMat(Mat4& m) { Position += m.get(Mat4::Row::i3); }
	
	void SetAmbient(const Vec4& v) { Ambient.set(v.x(), v.y(), v.z(), Ambient.w()); }
	void SetDiffuse(const Vec4& v) { Diffuse.set(v.x(), v.y(), v.z(), Diffuse.w()); }
	void SetSpecular(const Vec4& v) { Specular.set(v); }
	void SetSpecularExponent(float f) { Specular[w] = f; }
	void SetPosition(const Vec4& v) { Position.set(v); }
	void SetAttenuation(const Vec4& v) { Attenuation.set(v); }
	void SetDirection(const Vec4& v) { Direction.set(v.x(), v.y(), v.z(), Direction.w()); }
	void SetSpotExponent(float f) { Diffuse[w] = f; }
	void SetRange(float f) { Direction[w] = f; }

	Vec4 GetDirection() const { return Vec4(Direction.x(), Direction.y(), Direction.z()); }
	Vec4 GetPosition() const { return Position; }
	float GetRange() const { return Diffuse.w(); }
	float GetSpecularExponent() const { return Specular.w(); }
	float GetSpotExponent() const { return Diffuse.w(); }

#ifdef LIGHT_DEBUG	
	void DEBUG_Print(const char* const id);
#endif
};