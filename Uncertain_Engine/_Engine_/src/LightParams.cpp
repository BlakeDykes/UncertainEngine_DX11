#include "LightParams.h"

PhongADS_Directional::PhongADS_Directional(const PhongADS_Directional& l)
{
	float offset = this->Ambient[w];
	this->Ambient = l.Ambient;
	this->Ambient[w] = offset;

	this->Diffuse = l.Diffuse;
	this->Specular = l.Specular;
	this->Direction = l.Direction;
}

PhongADS_Directional& PhongADS_Directional::operator=(const PhongADS_Directional& l)
{
	if (this != &l)
	{
		float offset = this->Ambient[w];
		this->Ambient = l.Ambient;
		this->Ambient[w] = offset;

		this->Diffuse = l.Diffuse;
		this->Specular = l.Specular;
		this->Direction = l.Direction;
	}

	return *this;
}

void PhongADS_Directional::Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& direction)
{
	this->Ambient.set(ambient.x(), ambient.y(), ambient.z(), this->Ambient.w());
	this->Diffuse = diffuse;
	this->Specular = specular;
	this->Direction = direction;
}

void PhongADS_Directional::Clean()
{
	float offset = Ambient.w();
	memset(this, 0, sizeof(PhongADS_Directional));
	this->Ambient.set(0.0f, 0.0f, 0.0f, offset);
}

void PhongADS_Directional::Reflect(const Mat4& m)
{
	this->Direction *= m;
}




PhongADS_Point::PhongADS_Point(const PhongADS_Point& l)
{
	float offset = this->Ambient[w];
	this->Ambient = l.Ambient;
	this->Ambient[w] = offset;

	this->Diffuse = l.Diffuse;
	this->Specular = l.Specular;
	this->Position = l.Position;
	this->Attenuation = l.Attenuation;
}

PhongADS_Point& PhongADS_Point::operator=(const PhongADS_Point& l)
{
	if (this != &l)
	{
		float offset = this->Ambient[w];
		this->Ambient = l.Ambient;
		this->Ambient[w] = offset;

		this->Diffuse = l.Diffuse;
		this->Specular = l.Specular;
		this->Position = l.Position;
		this->Attenuation = l.Attenuation;
	}

	return *this;
}

void PhongADS_Point::Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& position, const Vec4& attenuation, float range)
{
	this->Ambient.set(ambient.x(), ambient.y(), ambient.z(), this->Ambient.w());
	this->Diffuse.set(diffuse.x(), diffuse.y(), diffuse.z(), range);
	this->Specular = specular;
	this->Position = position;
	this->Attenuation = attenuation;
}

void PhongADS_Point::Clean()
{
	float offset = Ambient.w();
	memset(this, 0, sizeof(PhongADS_Point));
	this->Ambient.set(0.0f, 0.0f, 0.0f, offset);
}

void PhongADS_Point::Reflect(const Mat4& m)
{
	this->Position *= m;
}

PhongADS_Spot::PhongADS_Spot(const PhongADS_Spot& l)
{
	float offset = this->Ambient[w];
	this->Ambient = l.Ambient;
	this->Ambient[w] = offset;

	this->Diffuse = l.Diffuse;
	this->Specular = l.Specular;
	this->Position = l.Position;
	this->Attenuation = l.Attenuation;
	this->Direction = l.Direction;
}
PhongADS_Spot& PhongADS_Spot::operator=(const PhongADS_Spot& l)
{
	if (this != &l)
	{
		float offset = this->Ambient[w];
		this->Ambient = l.Ambient;
		this->Ambient[w] = offset;

		this->Diffuse = l.Diffuse;
		this->Specular = l.Specular;
		this->Position = l.Position;
		this->Attenuation = l.Attenuation;
		this->Direction = l.Direction;
	}

	return *this;
}

void PhongADS_Spot::Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& position, const Vec4& attenuation, const Vec4& direction, float range, float spotExp)
{
	this->Ambient.set(ambient.x(), ambient.y(), ambient.z(), this->Ambient.w());
	this->Diffuse.set(diffuse.x(), diffuse.y(), diffuse.z(), spotExp);
	this->Specular = specular;
	this->Position = position;
	this->Attenuation = attenuation;
	this->Direction.set(direction.x(), direction.y(), direction.z(), range);
}

void PhongADS_Spot::Clean()
{
	float offset = Ambient.w();
	memset(this, 0, sizeof(PhongADS_Spot));
	this->Ambient.set(0.0f, 0.0f, 0.0f, offset);
}

void PhongADS_Spot::Reflect(const Mat4& m)
{
	this->Position *= m;

	float range = this->Direction.w();
	this->Direction[w] = 0.0f;
	this->Direction *= m;
	this->Direction[w] = range;;
}

#ifdef LIGHT_DEBUG	
void PhongADS_Directional::DEBUG_Print(const char* const id)
{
	DBOUT("++" << id << "\n");
	DBOUT("----\nAmbient : " << Ambient.x() << ", " << Ambient.y() << ", " << Ambient.z() << Ambient.w() << "\n");
	DBOUT("Diffuse   : " << Diffuse.x() << ", " << Diffuse.y() << ", " << Diffuse.z() << "\n");
	DBOUT("Specular  : " << Specular.x() << ", " << Specular.y() << ", " << Specular.z() << Specular.w() << "\n");
	DBOUT("Direction : " << Direction.x() << ", " << Direction.y() << ", " << Direction.z() << "\n----\n");

}

void PhongADS_Point::DEBUG_Print(const char* const id)
{
	DBOUT("++" << id << "\n");
	DBOUT("----\nAmbient  : " << Ambient.x() << ", " << Ambient.y() << ", " << Ambient.z() << "\n");
	DBOUT("Diffuse     : " << Diffuse.x() << ", " << Diffuse.y() << ", " << Diffuse.z() << "\n");
	DBOUT("Specular    : " << Specular.x() << ", " << Specular.y() << ", " << Specular.z() << Specular.w() << "\n");
	DBOUT("Position    : " << Position.x() << ", " << Position.y() << ", " << Position.z() << "\n");
	DBOUT("Attenuation : " << Attenuation.x() << ", " << Attenuation.y() << ", " << Attenuation.z() << "\n");
	DBOUT("Range       : " << Diffuse.w() << "\n----\n");
}


void PhongADS_Spot::DEBUG_Print(const char* const id)
{
	DBOUT("++" << id << "\n");
	DBOUT("----\nAmbient  : " << Ambient.x() << ", " << Ambient.y() << ", " << Ambient.z() << "\n");
	DBOUT("Diffuse     : " << Diffuse.x() << ", " << Diffuse.y() << ", " << Diffuse.z() << "\n");
	DBOUT("Specular    : " << Specular.x() << ", " << Specular.y() << ", " << Specular.z() << Specular.w() << "\n");
	DBOUT("Position    : " << Position.x() << ", " << Position.y() << ", " << Position.z() << "\n");
	DBOUT("Attenuation : " << Attenuation.x() << ", " << Attenuation.y() << ", " << Attenuation.z() << "\n");
	DBOUT("Direction   : " << Direction.x() << ", " << Direction.y() << ", " << Direction.z());
	DBOUT("Range       : " << Direction.w() << "\n----\n");
}
#endif