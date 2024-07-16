#pragma once

#include "SceneLightManager.h"
#include "LightController.h"

class LPController
{
public:
	void Update(PhongADS_Directional* light, const Vec4& v);
	void Update(PhongADS_Point* light, const Vec4& v);
	void Update(PhongADS_Spot* light, const Vec4& v);

	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const = 0;
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const = 0;
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const = 0;

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) = 0;
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v) = 0;
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) = 0;
};

class LPC_Ambient : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { vOut.set(light->Ambient); return true; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(light->Ambient); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(light->Ambient); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { if (light->Ambient.isEqual(v)) return false; else { light->SetAmbient(v); return true; } }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->Ambient.isEqual(v)) return false; else { light->SetAmbient(v); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->Ambient.isEqual(v)) return false; else { light->SetAmbient(v); return true; } }
};

class LPC_Diffuse : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { vOut.set(light->Diffuse); return true; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(light->Diffuse); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(light->Diffuse); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { if (light->Diffuse.isEqual(v)) return false; else { light->SetDiffuse(v); return true; } }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->Diffuse.isEqual(v)) return false; else { light->SetDiffuse(v); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->Diffuse.isEqual(v)) return false; else { light->SetDiffuse(v); return true; } }
};

class LPC_Specular : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { vOut.set(light->Specular); return true; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(light->Specular); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(light->Specular); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { if (light->Specular.isEqual(v)) return false; else { light->SetSpecular(v); return true; } }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->Specular.isEqual(v)) return false; else { light->SetSpecular(v); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->Specular.isEqual(v)) return false; else { light->SetSpecular(v); return true; } }
};

class LPC_SpecularExponent : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { vOut.set(0.0f, 0.0f, 0.0f, light->GetSpecularExponent()); return true; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(0.0f, 0.0f, 0.0f, light->GetSpecularExponent()); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(0.0f, 0.0f, 0.0f, light->GetSpecularExponent()); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->GetSpecularExponent() == v.w()) return false; else { light->SetSpecularExponent(v.w()); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->GetSpecularExponent() == v.w()) return false; else { light->SetSpecularExponent(v.w()); return true; } }
};

class LPC_Position : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(vOut); return false; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(light->Position); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(light->Position); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->Position.isEqual(v)) return false; else { light->SetPosition(v); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->Position.isEqual(v)) return false; else { light->SetPosition(v); return true; } }
};

class LPC_Direction : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { vOut.set(light->Direction); return true; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(vOut); return false; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(light->Direction); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { if (light->Direction.isEqual(v)) return false; else { light->SetDirection(v); return true; } }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->Direction.isEqual(v)) return false; else { light->SetDirection(v); return true; } }
};

class LPC_Attenuation : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(vOut); return false; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(light->Attenuation); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(light->Attenuation); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->Attenuation.isEqual(v)) return false; else { light->SetAttenuation(v); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->Attenuation.isEqual(v)) return false; else { light->SetAttenuation(v); return true; } }
};

class LPC_Range : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(vOut); return false; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { vOut.set(0.0f, 0.0f, 0.0f, light->GetRange()); return true; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(0.0f, 0.0f, 0.0f, light->GetRange()); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { if (light->GetRange() == v.w()) return false; else { light->SetRange(v.w()); return true; } }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->GetRange() == v.w()) return false; else { light->SetRange(v.w()); return true; } }
};

class LPC_SpotExponent : public LPController
{
public:
	virtual bool GetParam(PhongADS_Directional* light, Vec4& vOut) const override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(vOut); return false; }
	virtual bool GetParam(PhongADS_Point* light, Vec4& vOut) const override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(vOut); return false; }
	virtual bool GetParam(PhongADS_Spot* light, Vec4& vOut) const override { vOut.set(0.0f, 0.0f, 0.0f, light->GetSpotExponent()); return true; }

private:
	virtual bool privUpdate(PhongADS_Directional* light, const Vec4& v) override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Point* light, const Vec4& v)  override { AZUL_UNUSED_VAR(light); AZUL_UNUSED_VAR(v); return false; }
	virtual bool privUpdate(PhongADS_Spot* light, const Vec4& v) override { if (light->GetSpotExponent() == v.w()) return false; else { light->SetSpotExponent(v.w()); return true; } }
};

class LightParamController
{
public:
	static void UpdateReflection(PhongADS_Directional* light) { Instance()->pReflectionController->Update(light); }
	static void UpdateReflection(PhongADS_Point* light) { Instance()->pReflectionController->Update(light); }
	static void UpdateReflection(PhongADS_Spot* light) { Instance()->pReflectionController->Update(light); }

	static void Set(LightController::ReflectionController* reflection) { Instance()->pReflectionController = reflection; }

	static LPController* GetParamController(LightParameters param);

	static LPController* GetController_Ambient()			{ return &Instance()->Ambient; }
	static LPController* GetController_Diffuse()			{ return &Instance()->Diffuse; }
	static LPController* GetController_Specular()			{ return &Instance()->Specular; }
	static LPController* GetController_SpecularExponent()	{ return &Instance()->SpecularExponent; }
	static LPController* GetController_Position()			{ return &Instance()->Position; }
	static LPController* GetController_Direction()			{ return &Instance()->Direction; }
	static LPController* GetController_Attenuation()		{ return &Instance()->Attenuation; }
	static LPController* GetController_Range()				{ return &Instance()->Range; }
	static LPController* GetController_SpotExponent()		{ return &Instance()->SpotExponent; }

private:
	static LightParamController* Instance();
	static LightParamController* instance;

	LightController::ReflectionController* pReflectionController;
	LPC_Ambient				Ambient;
	LPC_Diffuse				Diffuse;
	LPC_Specular			Specular;
	LPC_SpecularExponent	SpecularExponent;
	LPC_Position			Position;
	LPC_Direction			Direction;
	LPC_Attenuation			Attenuation;
	LPC_Range				Range;
	LPC_SpotExponent		SpotExponent;
};















