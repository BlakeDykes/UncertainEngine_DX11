#ifndef VBO_DATA_H
#define VBO_DATA_H

#include "VBOData.pb.h"

class VBOData
{
public:
	enum class VBO_TARGET
	{
		ARRAY_BUFFER = 0,
		ELEMENT_ARRAY_BUFFER = 1,
		UNINITIALIZED = 3,
		DEFAULT = ARRAY_BUFFER
	};

	enum class VBO_COMPONENT
	{
		BYTE = 0,
		UNSIGNED_BYTE = 1,
		SHORT = 2,
		UNSIGNED_SHORT = 3,
		INT = 4,
		UNSIGNED_INT = 5,
		FLOAT = 6,
		DOUBLE = 7,
		UNINITIALIZED = 8,
		DEFAULT = FLOAT
	};

	enum class VBO_TYPE
	{
		VEC2 = 0,
		VEC3 = 1,
		VEC4 = 2,
		MAT2 = 3,
		MAT3 = 4,
		MAT4 = 5,
		SCALAR = 6,
		VECTOR = 7,
		MATRIX = 8,
		DEFAULT = VEC3
	};

	VBOData();
	VBOData(const VBOData& v);
	VBOData& operator=(const VBOData& v);
	~VBOData();

	void Serialize(VBOData_proto& out) const;
	void Deserialize(const VBOData_proto& in);

	void ConvertData(VBOData::VBO_COMPONENT componentReq, const unsigned char* pIndices, unsigned int stride);

	// UInt Data conversion
	void ConverttoUInt(const unsigned char* pIndices, unsigned int stride);
	void ConvertData_UShorttoUInt(const unsigned char* pIndices, unsigned int stride);
	void ConvertData_BytetoUInt(const unsigned char* pIndices, unsigned int stride);

	void Print(const char* pName) const;

	VBO_TARGET		TargetType;
	VBO_COMPONENT	ComponentType;
	VBO_TYPE		VboType;
	unsigned int	AttribIndex;
	unsigned int	Count;
	unsigned int	DataSize;
	bool			bIsEnabled;
	unsigned char*	poData = nullptr;
};

#endif