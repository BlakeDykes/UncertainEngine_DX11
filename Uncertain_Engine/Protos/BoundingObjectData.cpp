#include "BoundingObjectData.h"

using namespace Uncertain;

BoundingObjectData::BoundingObjectData()
	: BoundingType(BOUNDING_TYPE::UNINITIALIZED), poCenter(new Vec3()), Radius(-1.0f), AspectRatio(-1.0f)
{
}

BoundingObjectData::BoundingObjectData(float width, float height)
	: BoundingType(BOUNDING_TYPE::RECT), poCenter(new Vec3(width * 0.5f, height * 0.5f, -1.0f)),
	Radius(-1.0f), AspectRatio(width / height)
{
}


BoundingObjectData::BoundingObjectData(Vec3* v, unsigned int numVerts)
	: BoundingType(BOUNDING_TYPE::SPHERE), poCenter(new Vec3()), AspectRatio(-1.0f)
{
	assert(v);
	
	// Ritter Sphere
	this->SphereFromDistantPoints(v, numVerts);

	for (unsigned int i = 0; i < numVerts; i++)
	{
		this->ExpandSphereToFit(v[i]);
	}
}

BoundingObjectData::~BoundingObjectData()
{
	delete this->poCenter;
}

void BoundingObjectData::Serialize(BoundingObjectData_proto& out) const
{
	BoundingObjectData_proto::RectData* pRectData = nullptr;
	BoundingObjectData_proto::SphereData* pSphereData = nullptr;

	switch (this->BoundingType)
	{
	case BOUNDING_TYPE::RECT:
		out.set_boundingtype(BoundingObjectData_proto_BOUNDING_TYPE_RECT);
		pRectData = out.mutable_rectdata();
		pRectData->set_center_x(this->poCenter->x());
		pRectData->set_center_y(this->poCenter->y());
		pRectData->set_center_z(this->poCenter->z());
		pRectData->set_aspectratio(this->AspectRatio);
		break;
	
	case BOUNDING_TYPE::SPHERE:
		out.set_boundingtype(BoundingObjectData_proto_BOUNDING_TYPE_SPHERE);
		pSphereData = out.mutable_spheredata();
		pSphereData->set_center_x(this->poCenter->x());
		pSphereData->set_center_y(this->poCenter->y());
		pSphereData->set_center_z(this->poCenter->z());
		pSphereData->set_radius(this->Radius);
		break;

	case BOUNDING_TYPE::UNINITIALIZED:
	default:
		assert(false);
		break;
	}

}

void BoundingObjectData::Deserialize(BoundingObjectData_proto& in)
{
	BoundingObjectData_proto::RectData* pRectData = nullptr;
	BoundingObjectData_proto::SphereData* pSphereData = nullptr;


	switch (in.boundingtype())
	{
	case BoundingObjectData_proto_BOUNDING_TYPE_RECT:
		this->BoundingType = BOUNDING_TYPE::RECT;
		pRectData = in.mutable_rectdata();
		this->poCenter->set(pRectData->center_x(), pRectData->center_y(), pRectData->center_z());
		this->AspectRatio = pRectData->aspectratio();
		break;

	case BoundingObjectData_proto_BOUNDING_TYPE_SPHERE:
		this->BoundingType = BOUNDING_TYPE::SPHERE;
		pSphereData = in.mutable_spheredata();
		this->poCenter->set(pSphereData->center_x(), pSphereData->center_y(), pSphereData->center_z());
		this->Radius = pSphereData->radius();
		break;

	case BoundingObjectData_proto_BOUNDING_TYPE_BoundingObjectData_proto_BOUNDING_TYPE_INT_MAX_SENTINEL_DO_NOT_USE_:
	case BoundingObjectData_proto_BOUNDING_TYPE_BoundingObjectData_proto_BOUNDING_TYPE_INT_MIN_SENTINEL_DO_NOT_USE_:
	default:
		assert(false);
		break;
	}
}

void BoundingObjectData::MostSeparatedPoints(unsigned int& minIndex, unsigned int& maxIndex, Vec3* pt, unsigned int numPts)
{
	unsigned int minx = 0;
	unsigned int maxx = 0;
	unsigned int miny = 0;
	unsigned int maxy = 0;
	unsigned int minz = 0;
	unsigned int maxz = 0;

	for (unsigned int i = 0; i < numPts; i++)
	{
		if (pt[i][x] < pt[minx][x])
			minx = i;
		if (pt[i][x] > pt[maxx][x])
			maxx = i;
		if (pt[i][y] < pt[miny][y])
			miny = i;
		if (pt[i][y] > pt[maxy][y])
			maxy = i;
		if (pt[i][z] < pt[minz][z])
			minz = i;
		if (pt[i][z] > pt[maxz][z])
			maxz = i;
	}

	MinMaxData data[6];

	data[0].v.set(pt[minx]);
	data[1].v.set(pt[miny]);
	data[2].v.set(pt[minz]);
	data[3].v.set(pt[maxx]);
	data[4].v.set(pt[maxy]);
	data[5].v.set(pt[maxz]);

	data[0].index = minx;
	data[1].index = miny;
	data[2].index = minz;
	data[3].index = maxx;
	data[4].index = maxy;
	data[5].index = maxz;

	float maxDist = 0.0f;
	float tempDist;

	for (int i = 0; i < 6; i++)
	{
		for (int j = i + 1; j < 6; j++)
		{
			tempDist = (data[i].v - data[j].v).len();
			if (tempDist > maxDist)
			{
				maxDist = tempDist;
				minIndex = data[i].index;
				maxIndex = data[j].index;
			}
		}
	}
}

void BoundingObjectData::SphereFromDistantPoints(Vec3* pt, unsigned int numPts)
{
	unsigned int min = 0;
	unsigned int max = 0;
	this->MostSeparatedPoints(min, max, pt, numPts);

	*this->poCenter = (pt[min] + pt[max]) * 0.5f;
	this->Radius = sqrtf((pt[max] - *this->poCenter).dot(pt[max] - *this->poCenter));
}

void BoundingObjectData::ExpandSphereToFit(Vec3& p)
{
	float distance2 = (p - *this->poCenter).dot(p - *this->poCenter);

	if (distance2 > this->Radius * this->Radius)
	{
		float distance = sqrtf(distance2);
		float newRadius = (this->Radius + distance) * 0.5f;
		float centerAdjustment = (newRadius - this->Radius) / distance;
		this->Radius = newRadius;
		*this->poCenter += (p - *this->poCenter) * centerAdjustment;
	}
}

