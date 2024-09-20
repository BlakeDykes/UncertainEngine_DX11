#pragma once

#include "Vec3.h"
#include "BoundingData.pb.h"

class BoundingData
{
public:

	enum class TYPE
	{
		UNSPECIFIED = 0,
		RECT = 1,
		SPHERE = 2,
	};



	BoundingData();
	BoundingData(const BoundingData& m) = delete;
	BoundingData& operator=(const BoundingData& m) = delete;
	~BoundingData();

	BoundingData(Vec3* v, unsigned int numVerts);
	BoundingData(float width, float height);

	void Serialize(BoundingData_proto& out) const;
	void Deserialize(const BoundingData_proto& in);

	BoundingData::TYPE BoundingType;
	Vec3* poCenter;
	float Radius;
	float AspectRatio;


private:
	// Ritter Sphere
	void MostSeparatedPoints(unsigned int& minIndex, unsigned int& maxIndex, Vec3* pt, unsigned int numPts);
	void SphereFromDistantPoints(Vec3* pt, unsigned int numPts);
	void ExpandSphereToFit(Vec3& p);

};