#include "BoundingData.h"
#include "Trig.h"

BoundingData::BoundingData()
	: BoundingType(TYPE::UNSPECIFIED)
	, poCenter(new Vec3())
	, Radius(0.0f)
	, AspectRatio(0.0f)
{
}

BoundingData::~BoundingData()
{
	delete this->poCenter;
}

BoundingData::BoundingData(Vec3* v, unsigned int numVerts)
	: BoundingType(TYPE::SPHERE)
	, poCenter(new Vec3())
	, AspectRatio(0.0f)
{
	assert(v);

	// Ritter Sphere
	this->SphereFromDistantPoints(v, numVerts);

	for (unsigned int i = 0; i < numVerts; i++)
	{
		this->ExpandSphereToFit(v[i]);
	}
}

BoundingData::BoundingData(float width, float height)
	: BoundingType(TYPE::RECT)
	, poCenter(new Vec3(width * 0.5f, height * 0.5f, -1.0f))
	, Radius(-1.0f)
	, AspectRatio(width / height)
{
}

void BoundingData::Serialize(BoundingData_proto& out) const
{
	out.set_center_x(this->poCenter->x());
	out.set_center_y(this->poCenter->y());
	out.set_center_z(this->poCenter->z());

	switch (this->BoundingType)
	{
	case BoundingData::TYPE::RECT:
		out.set_bounding_object_type(BoundingData_proto_TYPE_TYPE_RECT);
		out.set_aspectratio(this->AspectRatio);
		break;

	case BoundingData::TYPE::SPHERE:
		out.set_bounding_object_type(BoundingData_proto_TYPE_TYPE_RECT);
		out.set_radius(this->Radius);

		break;
	case BoundingData::TYPE::UNSPECIFIED:
		out.set_bounding_object_type(BoundingData_proto_TYPE_TYPE_UNSPECIFIED);

		break;

	default:
		assert(false);
		break;
	}
}

void BoundingData::Deserialize(const BoundingData_proto& in)
{
	this->poCenter->set(in.center_x(), in.center_y(), in.center_z());
	this->Radius = in.radius();
	this->AspectRatio = in.aspectratio();

	BoundingData_proto_TYPE inType = in.bounding_object_type();

	switch (inType)
	{

	case BoundingData_proto_TYPE_TYPE_RECT:
		this->BoundingType = BoundingData::TYPE::RECT;
		break;
	case BoundingData_proto_TYPE_TYPE_SPHERE:
		this->BoundingType = BoundingData::TYPE::SPHERE;
		break;

	case BoundingData_proto_TYPE_TYPE_UNSPECIFIED:
		break;

	case BoundingData_proto_TYPE_BoundingData_proto_TYPE_INT_MAX_SENTINEL_DO_NOT_USE_:
	case BoundingData_proto_TYPE_BoundingData_proto_TYPE_INT_MIN_SENTINEL_DO_NOT_USE_:
	default:
		assert(false);
		break;
	}

}

void BoundingData::MostSeparatedPoints(unsigned int& minIndex, unsigned int& maxIndex, Vec3* pt, unsigned int numPts)
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

	struct minMaxData
	{
		Vec3 v;
		unsigned int index;
		int pad[3];
	};

	minMaxData data[6];

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

void BoundingData::SphereFromDistantPoints(Vec3* pt, unsigned int numPts)
{
	unsigned int min = 0;
	unsigned int max = 0;
	this->MostSeparatedPoints(min, max, pt, numPts);

	*this->poCenter = (pt[min] + pt[max]) * 0.5f;
	this->Radius = Trig::sqrt((pt[max] - *this->poCenter).dot(pt[max] - *this->poCenter));
}

void BoundingData::ExpandSphereToFit(Vec3& p)
{
	float distance2 = (p - *this->poCenter).dot(p - *this->poCenter);

	if (distance2 > this->Radius * this->Radius)
	{
		float distance = Trig::sqrt(distance2);
		float newRadius = (this->Radius + distance) * 0.5f;
		float centerAdjustment = (newRadius - this->Radius) / distance;
		this->Radius = newRadius;
		*this->poCenter += (p - *this->poCenter) * centerAdjustment;
	}
}
