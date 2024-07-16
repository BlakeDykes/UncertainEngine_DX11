#pragma once

#include "MaterialObject.h"

class MaterialObject_NULL : public MaterialObject
{
public:

	MaterialObject_NULL()
		: MaterialObject(MATTYPE_NULL::NULL_MATERIAL)
	{}

	virtual void Set(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular) override { ALIGN_UNUSED_VAR(ambient); ALIGN_UNUSED_VAR(diffuse); ALIGN_UNUSED_VAR(specular);
	}

	virtual void SetAmbient(const Vec4& v) override  { ALIGN_UNUSED_VAR(v);   }
	virtual void SetDiffuse(const Vec4& v) override  { ALIGN_UNUSED_VAR(v);   }
	virtual void SetSpecular(const Vec4& v) override { ALIGN_UNUSED_VAR(v);   }
	virtual void SetSpecularExp(float exp) override  { ALIGN_UNUSED_VAR(exp); }

};
