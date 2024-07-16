#include "MaterialObject.h"

MaterialObject::MaterialObject()
	: poLightParameters(new MatLightParameters())
{
}

MaterialObject::MaterialObject(MATTYPE_NULL nullSpecifier)
	: poLightParameters(nullptr)
{
	ALIGN_UNUSED_VAR(nullSpecifier);
}

const char* MaterialObject::GetName()
{
	return nullptr;
}

void MaterialObject::Print()
{
}

MaterialObject::~MaterialObject()
{
	delete poLightParameters;
}

void MaterialObject::Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular)
{
	this->poLightParameters->Set(ambient, diffuse, specular);
}

void MaterialObject::Set(const Vec4& flatColor)
{
	this->poLightParameters->Set(flatColor);
}

bool MaterialObject::Compare(NodeBase& pNode)
{
	return this->GetHandle() == static_cast<MaterialObject*>(&pNode)->GetHandle();
}

void MaterialObject::MatLightParameters::Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular)
{
	Ambient = ambient;
	Diffuse = diffuse;
	Specular = specular;
}

void MaterialObject::MatLightParameters::Set(const Vec4& flatColor)
{
	Ambient = flatColor;
	Diffuse.clear();
	Specular.clear();

}
