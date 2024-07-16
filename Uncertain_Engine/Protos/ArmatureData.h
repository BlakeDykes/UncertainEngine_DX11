#ifndef ARMATURE_DATA_H
#define ARMATURE_DATA_H

#include "ArmatureData.pb.h"

class ArmatureData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;
	static const char* FILE_PREFIX;

	ArmatureData();
	ArmatureData(const ArmatureData& m) = delete;
	ArmatureData& operator=(const ArmatureData& m) = delete;
	~ArmatureData();

	void Serialize(ArmatureData_proto& out) const;
	void Deserialize(ArmatureData_proto& in);

	char pArmatureName[FILE_NAME_SIZE];
	unsigned int JointDepth;
	unsigned int JointCount;
	unsigned int* poJointTable;
};

#endif // !ARMATURE_DATA_H
