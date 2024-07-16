#pragma once

#include "DLink.h"
#include "Vec4.h"

class MaterialObject : public DLink
{
public:
	MaterialObject();

	virtual ~MaterialObject();

	struct MatLightParameters : public Align16
	{
		Vec4 Ambient;
		Vec4 Diffuse;
		Vec4 Specular;

		MatLightParameters() = default;
		void Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular);
		void Set(const Vec4& flatColor);
	};

	virtual void Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular);
	virtual void Set(const Vec4& flatColor);
	const MatLightParameters* GetLightParams() const { return poLightParameters; }

	virtual void SetAmbient(const Vec4& v) { poLightParameters->Ambient.set(v); }
	virtual void SetDiffuse(const Vec4& v) { poLightParameters->Diffuse.set(v); }
	virtual void SetSpecular(const Vec4& v) { poLightParameters->Specular.set(v); }
	virtual void SetSpecularExp(float exp) { poLightParameters->Specular.set(
		poLightParameters->Specular.x(), poLightParameters->Specular.y(), poLightParameters->Specular.z(), exp); }

	const Vec4& GetAmbient() const { return poLightParameters->Ambient; }
	const Vec4& GetDiffuse() const { return poLightParameters->Diffuse; }
	const Vec4& GetSpecular() const { return poLightParameters->Specular; }

	virtual bool Compare(NodeBase& pNode) override;

private:
	MatLightParameters* poLightParameters;

protected:

	enum MATTYPE_NULL
	{
		NULL_MATERIAL
	};

	MaterialObject(MATTYPE_NULL nullSpecifier);


	// Inherited via NodeBase
	const char* GetName() override;

	void Print() override;

};