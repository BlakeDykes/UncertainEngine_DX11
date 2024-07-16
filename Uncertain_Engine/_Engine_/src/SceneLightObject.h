#pragma once

#include "DLink.h"
#include "Vec4.h"
#include "Mat4.h"
#include "LightParams.h"
#include "ShaderManager.h"
#include "Utils.h"

class LightController;
class LightCommand;

enum LightType
{
	Directional,
	Point,
	Spot
};

enum class LightParameters
{
	Ambient,
	Diffuse,
	Specular,
	SpecularExponent,
	Direction,
	Position,
	Attenuation,
	Range,
	SpotExponent
};

struct LightCount
{
	unsigned int DirectionalCount : 3;
	unsigned int PointCount : 3;
	unsigned int SpotCount : 3;
	unsigned int res_pad : 7;

	LightCount();
	LightCount(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount);
	LightCount(const LightCount& l);
	LightCount& operator=(const LightCount& l);
	~LightCount() = default;

	void Set(unsigned int dirCount, unsigned int pointCount, unsigned int spotCount);
};

struct UpdatedLight
{
	LightType LType;
	size_t LIndex;

	UpdatedLight(LightType lType, size_t lIndex)
		: LType(lType), LIndex(lIndex)
	{ }
	UpdatedLight(const UpdatedLight&) = default;
	UpdatedLight& operator=(const UpdatedLight&) = default;
	~UpdatedLight() = default;

	bool isEqual(LightType lType, size_t lIndex) { return LType == lType && lIndex == LIndex; }
};

class LightIterator
{
public:
	LightIterator();

	size_t GetIndex(PhongADS_Directional* pLight);
	size_t GetIndex(PhongADS_Point* pLight);
	size_t GetIndex(PhongADS_Spot* pLight);

	PhongADS_Directional*& GetDirectional(size_t index) { return pDirectionals[index]; }
	PhongADS_Point*& GetPoint(size_t index) { return pPoints[index]; }
	PhongADS_Spot*& GetSpot(size_t index) { return pSpots[index]; }

	size_t GetDirectionalCount() const { return pDirectionals.size(); }
	size_t GetPointCount() const { return pPoints.size(); }
	size_t GetSpotCount() const { return pSpots.size(); }

	void Push_Directional(PhongADS_Directional*& pLight) { pDirectionals.push_back(pLight); }
	void Push_Point(PhongADS_Point*& pLight) { pPoints.push_back(pLight); }
	void Push_Spot(PhongADS_Spot*& pLight) { pSpots.push_back(pLight); }
	PhongADS_Directional* Pop_Directional();
	PhongADS_Point* Pop_Point();
	PhongADS_Spot* Pop_Spot();

	void Duplicate(LightIterator* it, unsigned char*& pNewData);

private:
	std::vector<PhongADS_Directional*> pDirectionals;
	std::vector<PhongADS_Point*> pPoints;
	std::vector<PhongADS_Spot*> pSpots;

	unsigned int Front_Dir;
	unsigned int Front_Point;
	unsigned int Front_Spot;
};

class SceneLightObject : public DLink
{
public:

	SceneLightObject();
	virtual ~SceneLightObject();

	void Update();

	void Set(unsigned int dir_allocateCount, unsigned int point_allocateCount, unsigned int spot_allocateCount);
	void Set(LightCount lightCount);
	
	PhongADS_Directional* CreateDirectional( 
		const Vec4& ambient = PHONG_DEFAULT_AMBIENT,
		const Vec4& diffuse = PHONG_DEFAULT_DIFFUSE,
		const Vec4& specular = PHONG_DEFAULT_SPECULAR,
		const Vec4& direction = PHONG_DEFAULT_DIRECTION
	);

	PhongADS_Point* CreatePoint(
		const Vec4& ambient = PHONG_DEFAULT_AMBIENT,
		const Vec4& diffuse = PHONG_DEFAULT_DIFFUSE,
		const Vec4& specular = PHONG_DEFAULT_SPECULAR,
		const Vec4& pos = PHONG_DEFAULT_POSITION,
		const Vec4& attenuation = PHONG_DEFAULT_ATTENUATION,
		float range = PHONG_DEFAULT_RANGE
	);

	PhongADS_Spot* CreateSpot(
		const Vec4& ambient = PHONG_DEFAULT_AMBIENT,
		const Vec4& diffuse = PHONG_DEFAULT_DIFFUSE,
		const Vec4& specular = PHONG_DEFAULT_SPECULAR,
		const Vec4& pos = PHONG_DEFAULT_POSITION,
		const Vec4& attenuation = PHONG_DEFAULT_ATTENUATION,
		const Vec4& direction = PHONG_DEFAULT_DIRECTION,
		float range = PHONG_DEFAULT_RANGE,
		float spotexp = PHONG_DEFAULT_SPOT_EXP
	);

	void ReceiveLightCommand(LightCommand* pLC);

	void SetDeltaPos(PhongADS_Point* pLight, const Vec4& deltaPos, FrameModifier::ModType mType);
	void SetDeltaPos(PhongADS_Spot* pLight, const Vec4& deltaPos, FrameModifier::ModType mType);

	void UpdateWorld_Dir(size_t index, const Vec4& rot, const Vec4& pos);
	void UpdateWorld_Point(size_t index, const Vec4& rot, const Vec4& pos);
	void UpdateWorld_Spot(size_t index, const Vec4& rot, const Vec4& pos);

	void TurnOff_Dir(size_t index, float& specCacheOut);
	void TurnOff_Point(size_t index, float& specCacheOut);
	void TurnOff_Spot(size_t index, float& specCacheOut);

	void TurnOn_Dir(size_t index, float specVal);
	void TurnOn_Point(size_t index, float specVal);
	void TurnOn_Spot(size_t index, float specVal);

	size_t GetIndex(PhongADS_Directional* light) { return poIterator->GetIndex(light); }
	size_t GetIndex(PhongADS_Point* light) { return poIterator->GetIndex(light); }
	size_t GetIndex(PhongADS_Spot* light) { return poIterator->GetIndex(light); }

	void Destroy(PhongADS_Directional*& pLight);
	void Destroy(PhongADS_Point*& pLight);
	void Destroy(PhongADS_Spot*& pLight);

	const unsigned char& GetPhongData() const { return *poData; }
	const unsigned char& GetReflected(const Mat4 & m);
	UINT GetMaxSize() const { return DataSize; }

	bool HasChanged() const { return bHasChanged; }

	virtual bool Compare(NodeBase& pNode) override;

#ifdef LIGHT_DEBUG
	void DEBUG_Print(PhongADS_Directional* light, const char* const id);
	void DEBUG_Print(PhongADS_Point* light, const char* const id);
	void DEBUG_Print(PhongADS_Spot* light, const char* const id);
#endif

private:
	LightIterator* poIterator;
	LightController* poController;


protected:
	LightCount* poMaxCount;
	size_t DataSize;
	unsigned char* poData;
	bool bHasChanged;

	void InitializeLightData();
};


