#ifndef BOUNDING_SPHERE_DATA
#define BOUNDING_SPHERE_DATA

#include "MathEngine.h"
#include "BoundingObjectData.pb.h"

using namespace Uncertain;

class BoundingObjectData
{
public:

	enum class BOUNDING_TYPE
	{
		RECT	= 0,
		SPHERE	= 1,

		UNINITIALIZED = 2
	};

	struct MinMaxData
	{
		Vec3 v;
		unsigned int index;
		int pad[3];
	};

	BoundingObjectData();
	BoundingObjectData(const BoundingObjectData& m) = delete;
	BoundingObjectData& operator=(const BoundingObjectData& m) = delete;
	~BoundingObjectData();

	BoundingObjectData(Vec3* v, unsigned int numVerts);
	BoundingObjectData(float width, float height);

	void Serialize(BoundingObjectData_proto& out) const;
	void Deserialize(BoundingObjectData_proto& in);
	
	BOUNDING_TYPE BoundingType;
	Vec3* poCenter;

	// Sphere Data
	float Radius;

	// Rect Data
	float AspectRatio;

private:

	// Ritter Sphere
	void MostSeparatedPoints(unsigned int& minIndex, unsigned int& maxIndex, Vec3* pt, unsigned int numPts);
	void SphereFromDistantPoints(Vec3* pt, unsigned int numPts);
	void ExpandSphereToFit(Vec3& p);
};

#endif // !BOUNDING_SPHERE_DATA
