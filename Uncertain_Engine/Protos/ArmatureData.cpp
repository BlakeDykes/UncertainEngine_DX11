#include "ArmatureData.h"

const char* ArmatureData::FILE_PREFIX = "arm_";

ArmatureData::ArmatureData()
	: pArmatureName{0}, JointDepth(0), JointCount(0), poJointTable(nullptr)
{
}

ArmatureData::~ArmatureData()
{
	if (poJointTable)
	{
		delete[] poJointTable;
		poJointTable = nullptr;
	}
}

void ArmatureData::Serialize(ArmatureData_proto& out) const
{
	std::string armName((const char*)this->pArmatureName, FILE_NAME_SIZE);
	out.set_name(armName);

	out.set_jointcount(this->JointCount);
	out.set_jointdepth(this->JointDepth);

	size_t jointTableSize = JointCount * JointDepth * sizeof(unsigned int);

	std::string jointTableData((const char*)poJointTable, jointTableSize);

	out.set_jointtable(jointTableData);
}

void ArmatureData::Deserialize(ArmatureData_proto& in)
{
	memcpy_s(this->pArmatureName, FILE_NAME_SIZE, in.name().data(), FILE_NAME_SIZE);
	
	if (poJointTable)
	{
		delete[] poJointTable;
		poJointTable = nullptr;
	}

	this->JointCount = in.jointcount();
	this->JointDepth = in.jointdepth();

	size_t jointTableSize = JointCount * JointDepth;

	this->poJointTable = new unsigned int[jointTableSize]();
	memcpy_s(poJointTable, jointTableSize * sizeof(unsigned int), in.jointtable().data(), jointTableSize * sizeof(unsigned int));
}
