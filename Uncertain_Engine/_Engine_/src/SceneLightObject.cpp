#include "SceneLightObject.h"
#include "LightController.h"
#include "_LightCommandWrapper.h"

SceneLightObject::SceneLightObject()
	: poIterator(new LightIterator()), poController(nullptr), 
	poMaxCount(new LightCount()), DataSize(0), poData(nullptr), bHasChanged(false)
{
}

SceneLightObject::~SceneLightObject()
{
	delete this->poController;

	if (poData)
	{
		delete[] poData;
	}

	delete poMaxCount;
	delete poIterator;
}

void SceneLightObject::Update()
{
	this->poController->Update();

	bHasChanged = false;
}

void SceneLightObject::Set(unsigned int dir_allocateCount, unsigned int point_allocateCount, unsigned int spot_allocateCount)
{
	poMaxCount->Set(dir_allocateCount, point_allocateCount, spot_allocateCount);
	InitializeLightData();
	this->poController = new LightController(*this, this->poIterator);
}

void SceneLightObject::Set(LightCount lightCount)
{
	*poMaxCount = lightCount;
	InitializeLightData();
	this->poController = new LightController(*this, this->poIterator);
}

PhongADS_Directional* SceneLightObject::CreateDirectional(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& direction)
{
	if (poMaxCount->DirectionalCount <= 0) return nullptr;

	PhongADS_Directional* pLight = poIterator->Pop_Directional();
	
	pLight->Set(ambient, diffuse, specular, direction);

	bHasChanged = true;

	return pLight;
}

PhongADS_Point* SceneLightObject::CreatePoint(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& pos, const Vec4& attenuation, float range)
{
	if (poMaxCount->PointCount <= 0) return nullptr;

	PhongADS_Point* pLight = poIterator->Pop_Point();

	pLight->Set(ambient, diffuse, specular, pos, attenuation, range);

	bHasChanged = true;

	return pLight;
}

PhongADS_Spot* SceneLightObject::CreateSpot(const Vec4& ambient, const Vec4& diffuse, const Vec4& specular, const Vec4& pos, const Vec4& attenuation, const Vec4& direction, float range, float spotexp)
{
	if (poMaxCount->SpotCount <= 0) return nullptr;

	PhongADS_Spot* pLight = poIterator->Pop_Spot();

	pLight->Set(ambient, diffuse, specular, pos, attenuation, direction, range, spotexp);

	bHasChanged = true;

	return pLight;
}

void SceneLightObject::ReceiveLightCommand(LightCommand* pLC)
{
	poController->ReceiveLightCommand(pLC);
}

void SceneLightObject::SetDeltaPos(PhongADS_Point* pLight, const Vec4& deltaPos, FrameModifier::ModType mType)
{
	this->poController->ReceiveLightCommand((LightCommand*)new LC_DeltaPos_Point(pLight, Vec3(deltaPos), mType));
}

void SceneLightObject::SetDeltaPos(PhongADS_Spot* pLight, const Vec4& deltaPos, FrameModifier::ModType mType)
{
	this->poController->ReceiveLightCommand((LightCommand*)new LC_DeltaPos_Spot(pLight, Vec3(deltaPos), mType));
}

void SceneLightObject::UpdateWorld_Dir(size_t index, const Vec4& rot, const Vec4& pos)
{
	poController->UpdateWorld(poIterator->GetDirectional(index), rot, pos);
}

void SceneLightObject::UpdateWorld_Point(size_t index, const Vec4& rot, const Vec4& pos)
{
	poController->UpdateWorld(poIterator->GetPoint(index), rot, pos);
}

void SceneLightObject::UpdateWorld_Spot(size_t index, const Vec4& rot, const Vec4& pos)
{
	poController->UpdateWorld(poIterator->GetSpot(index), rot, pos);
}

void SceneLightObject::TurnOff_Dir(size_t index, float& specCacheOut)
{
	poController->TurnOff(poIterator->GetDirectional(index), specCacheOut);
}

void SceneLightObject::TurnOff_Point(size_t index, float& specCacheOut)
{
	poController->TurnOff(poIterator->GetPoint(index), specCacheOut);
}

void SceneLightObject::TurnOff_Spot(size_t index, float& specCacheOut)
{
	poController->TurnOff(poIterator->GetSpot(index), specCacheOut);
}

void SceneLightObject::TurnOn_Dir(size_t index, float specVal)
{
	poController->TurnOn(poIterator->GetDirectional(index), specVal);
}

void SceneLightObject::TurnOn_Point(size_t index, float specVal)
{
	poController->TurnOn(poIterator->GetPoint(index), specVal);
}

void SceneLightObject::TurnOn_Spot(size_t index, float specVal)
{
	poController->TurnOn(poIterator->GetSpot(index), specVal);
}

void SceneLightObject::Destroy(PhongADS_Directional*& pLight)
{
	assert((unsigned char*)pLight == poData + pLight->GetOffset());
	pLight->Clean();

	bHasChanged = true;
}

void SceneLightObject::Destroy(PhongADS_Point*& pLight)
{
	assert((unsigned char*)pLight == poData + pLight->GetOffset());
	pLight->Clean();

	bHasChanged = true;

}

void SceneLightObject::Destroy(PhongADS_Spot*& pLight)
{
	assert((unsigned char*)pLight == poData + pLight->GetOffset());
	pLight->Clean();

	bHasChanged = true;

}

const unsigned char& SceneLightObject::GetReflected(const Mat4& m) { return poController->GetReflected(m); }

bool SceneLightObject::Compare(NodeBase& pNode)
{
	return this->GetHandle() == static_cast<SceneLightObject*>(&pNode)->GetHandle();
}

void SceneLightObject::InitializeLightData()
{
	this->DataSize = 
		(poMaxCount->DirectionalCount * sizeof(PhongADS_Directional)) +
		(poMaxCount->PointCount * sizeof(PhongADS_Point)) +
		(poMaxCount->SpotCount * sizeof(PhongADS_Spot));


	// Heap allocation for max number of lights
	this->poData = new unsigned char[DataSize];
	memset(poData, 0x0, DataSize);

	size_t offset = 0;

	AZUL_PLACEMENT_NEW_BEGIN
	#undef new 

	// Placement news for PhongADS_Directional
	for (unsigned int i = 0; i < poMaxCount->DirectionalCount; ++i)
	{
		PhongADS_Directional* pNew = new(poData + offset) PhongADS_Directional(offset);
		this->poIterator->Push_Directional(pNew);
		offset += sizeof(PhongADS_Directional);
	}

	for (unsigned int i = 0; i < poMaxCount->PointCount; ++i)
	{
		PhongADS_Point* pNew = new(poData + offset) PhongADS_Point(offset);
		this->poIterator->Push_Point(pNew);
		offset += sizeof(PhongADS_Point);
	}

	// Placement news for PhongADS_Spot
	for (unsigned int i = 0; i < poMaxCount->SpotCount; ++i)
	{
		PhongADS_Spot* pNew = new(poData + offset) PhongADS_Spot(offset);
		this->poIterator->Push_Spot(pNew);
		offset += sizeof(PhongADS_Spot);
	}

	AZUL_PLACEMENT_NEW_END
}

LightIterator::LightIterator()
	: pDirectionals(), pPoints(), pSpots(),
	Front_Dir(0), Front_Point(0), Front_Spot(0)
{
}

size_t LightIterator::GetIndex(PhongADS_Directional* pLight)
{
	return size_t(pLight->GetOffset() / sizeof(PhongADS_Directional));
}

size_t LightIterator::GetIndex(PhongADS_Point* pLight)
{
	return size_t(
		(pLight->GetOffset() 
		- (sizeof(PhongADS_Directional) * pDirectionals.size())
		)/ sizeof(PhongADS_Point)
	);
}

size_t LightIterator::GetIndex(PhongADS_Spot* pLight)
{
	return size_t(
		(pLight->GetOffset() 
		- (sizeof(PhongADS_Directional) * pDirectionals.size())
		- (sizeof(PhongADS_Point) * pPoints.size())
		)/ sizeof(PhongADS_Point)
	);
}

PhongADS_Directional* LightIterator::Pop_Directional()
{
	PhongADS_Directional* pLight = GetDirectional(Front_Dir);

	pLight->Clean();

	Front_Dir = Front_Dir < pDirectionals.size() ? Front_Dir + 1 : 0;

	return pLight;
}

PhongADS_Point* LightIterator::Pop_Point()
{
	PhongADS_Point* pLight = GetPoint(Front_Point);

	pLight->Clean();

	Front_Point = Front_Point < pPoints.size() ? Front_Point + 1 : 0;

	return pLight;
}

PhongADS_Spot* LightIterator::Pop_Spot()
{
	PhongADS_Spot* pLight = GetSpot(Front_Spot);

	pLight->Clean();

	Front_Spot = Front_Spot < pPoints.size() ? Front_Spot + 1 : 0;

	return pLight;
}

void LightIterator::Duplicate(LightIterator* it, unsigned char*& pNewData)
{
	std::vector<PhongADS_Directional*>::iterator dirIt;
	size_t offset = 0;
	for (dirIt = it->pDirectionals.begin(); dirIt != it->pDirectionals.end(); dirIt++)
	{
		this->pDirectionals.push_back((PhongADS_Directional*)(pNewData + offset));
		offset += sizeof(PhongADS_Directional);
	}
	std::vector<PhongADS_Point*>::iterator pointIt;
	for (pointIt = it->pPoints.begin(); pointIt != it->pPoints.end(); pointIt++)
	{
		this->pPoints.push_back((PhongADS_Point*)(pNewData + offset));
		offset += sizeof(PhongADS_Point);
	}
	std::vector<PhongADS_Spot*>::iterator spotIt;
	for (spotIt = it->pSpots.begin(); spotIt != it->pSpots.end(); spotIt++)
	{
		this->pSpots.push_back((PhongADS_Spot*)(pNewData + offset));
		offset += sizeof(PhongADS_Spot);
	}
}


LightCount::LightCount()
	: DirectionalCount(0), PointCount(0), SpotCount(0), res_pad(0)
{ }

LightCount::LightCount(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount)
	: DirectionalCount(dirCount > 7 ? 7 : dirCount), PointCount(pointCount > 7 ? 7 : pointCount),
	SpotCount(spotCount > 7 ? 7 : spotCount), res_pad(0)
{
}

LightCount::LightCount(const LightCount& l)
{
	DirectionalCount = l.DirectionalCount;
	PointCount = l.PointCount;
	SpotCount = l.SpotCount;
	res_pad = l.res_pad;
}

LightCount& LightCount::operator=(const LightCount& l)
{
	if (this != &l)
	{
		DirectionalCount = l.DirectionalCount;
		PointCount = l.PointCount;
		SpotCount = l.SpotCount;
		res_pad = l.res_pad;
	}

	return *this;
}

void LightCount::Set(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount)
{
	this->DirectionalCount = dirCount;
	this->PointCount = pointCount;
	this->SpotCount = spotCount;
}

#ifdef LIGHT_DEBUG

void SceneLightObject::DEBUG_Print(PhongADS_Directional* light, const char* const id)
{
	this->poController->ReceiveLightCommand(new LC_Debug_Dir(light, id));
}

void SceneLightObject::DEBUG_Print(PhongADS_Point* light, const char* const id)
{
	this->poController->ReceiveLightCommand(new LC_Debug_Point(light, id));
}

void SceneLightObject::DEBUG_Print(PhongADS_Spot* light, const char* const id)
{
	this->poController->ReceiveLightCommand(new LC_Debug_Spot(light, id));
}

#endif