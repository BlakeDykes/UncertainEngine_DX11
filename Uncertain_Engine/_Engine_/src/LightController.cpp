#include "LightController.h"
#include "SceneLightObject.h"
#include "LightParamController.h"

static Mat4 IDENTITYMAT(Mat4::IDENTITY_MAT4);

LightController::LightController(SceneLightObject& lightObj, LightIterator*& it)
	: LightObject(lightObj), pIt(it), poReflection(new ReflectionController(*this, LightObject.GetMaxSize(), it))
{
	LightParamController::Set(poReflection);
}

LightController::~LightController()
{
	delete poReflection;

	for (LightCommand* lc : this->oLightCommands)
	{
		delete lc;
	}
}

void LightController::ReceiveLightCommand(LightCommand* lc)
{
	this->oLightCommands.push_back(lc);
}

void LightController::UpdateWorld(PhongADS_Directional* pLight, const Vec4& rot, const Vec4& pos)
{
	ALIGN_UNUSED_VAR(pos);
	pLight->SetDirection(rot);

	poReflection->Update(pLight);
}

void LightController::UpdateWorld(PhongADS_Point* pLight, const Vec4& rot, const Vec4& pos)
{
	ALIGN_UNUSED_VAR(rot);
	pLight->SetPosition(pos);

	poReflection->Update(pLight);
}

void LightController::UpdateWorld(PhongADS_Spot* pLight, const Vec4& rot, const Vec4& pos)
{
	pLight->SetDirection(rot);
	pLight->SetPosition(pos);

	poReflection->Update(pLight);
}

void LightController::TurnOff(PhongADS_Directional* pLight, float& specCacheOut)
{
	pLight->TurnOff(specCacheOut);

	poReflection->Update(pLight);
}

void LightController::TurnOff(PhongADS_Point* pLight, float& specCacheOut)
{
	pLight->TurnOff(specCacheOut);

	poReflection->Update(pLight);
}

void LightController::TurnOff(PhongADS_Spot* pLight, float& specCacheOut)
{
	pLight->TurnOff(specCacheOut);

	poReflection->Update(pLight);
}

void LightController::TurnOn(PhongADS_Directional* pLight, float specVal)
{
	pLight->TurnOn(specVal);

	poReflection->Update(pLight);
}

void LightController::TurnOn(PhongADS_Point* pLight, float specVal)
{
	pLight->TurnOn(specVal);

	poReflection->Update(pLight);
}

void LightController::TurnOn(PhongADS_Spot* pLight, float specVal)
{
	pLight->TurnOn(specVal);

	poReflection->Update(pLight);
}

void LightController::Update()
{
	if (LightObject.HasChanged())
	{
		poReflection->RefreshData(LightObject.GetPhongData(), LightObject.GetMaxSize());
	}

	for (LightCommand* lc : this->oLightCommands)
	{
		lc->Update(this->poReflection);
	}
}

void LightController::RefreshData(unsigned char*& dest) const
{
	memcpy_s(dest, LightObject.GetMaxSize(), &(LightObject.GetPhongData()), LightObject.GetMaxSize());
}

const unsigned char& LightController::ReflectionController::Update(const Mat4& m)
{
	if (&m == pLastReflection) return *poReflected;

	if (pLastReflection != nullptr)
	{
		Controller.RefreshData(this->poReflected);
	}

	size_t count = poIt->GetDirectionalCount();
	for(size_t i = 0; i < count; ++i)
	{
		poIt->GetDirectional(i)->Reflect(m);
	}

	count = poIt->GetPointCount();
	for(size_t i = 0; i < count; ++i)
	{
		poIt->GetPoint(i)->Reflect(m);
	}

	count = poIt->GetSpotCount();
	for (size_t i = 0; i < count; ++i)
	{
		poIt->GetSpot(i)->Reflect(m);
	}


	pLastReflection = &m;

	return *this->poReflected;
}

void LightController::ReflectionController::Update(PhongADS_Directional* light)
{
	PhongADS_Directional* pLight = poIt->GetDirectional(Controller.GetIndex(light));
	assert(pLight);

	*pLight = *light;

	pLight->Reflect(*pLastReflection);
}

void LightController::ReflectionController::Update(PhongADS_Point* light)
{
	PhongADS_Point* pLight = poIt->GetPoint(Controller.GetIndex(light));
	assert(pLight);

	*pLight = *light;

	pLight->Reflect(*pLastReflection);
}

void LightController::ReflectionController::Update(PhongADS_Spot* light)
{
	PhongADS_Spot* pLight = poIt->GetSpot(Controller.GetIndex(light));
	assert(pLight);

	*pLight = *light;

	pLight->Reflect(*pLastReflection);
}



LightController::ReflectionController::ReflectionController(LightController& controller, size_t dataSize, LightIterator* it)
	: pLastReflection(&IDENTITYMAT), poIt(new LightIterator()), Controller(controller)
{
	this->poReflected = new unsigned char[dataSize];

	this->poIt->Duplicate(it, poReflected);
}

LightController::ReflectionController::~ReflectionController()
{
	delete[] this->poReflected;
	delete this->poIt;
}

void LightController::ReflectionController::RefreshData(const unsigned char& data, size_t dataSize)
{
	memcpy_s(this->poReflected, dataSize, &data, dataSize);
}

#ifdef LIGHT_DEBUG	


void LightController::ReflectionController::DEBUG_Print(PhongADS_Directional* pLight, const char* const id)
{
	DBOUT("~~~ Reflection ~~~\n");

	size_t index = Controller.GetIndex(pLight);
	poIt->GetDirectional(index)->DEBUG_Print(id);
}

void LightController::ReflectionController::DEBUG_Print(PhongADS_Point* pLight, const char* const id)
{
	DBOUT("~~~ Reflection ~~~\n");

	size_t index = Controller.GetIndex(pLight);
	poIt->GetPoint(index)->DEBUG_Print(id);
}

void LightController::ReflectionController::DEBUG_Print(PhongADS_Spot* pLight, const char* const id)
{
	DBOUT("~~~ Reflection ~~~\n");

	size_t index = Controller.GetIndex(pLight);
	poIt->GetSpot(index)->DEBUG_Print(id);
}

#endif