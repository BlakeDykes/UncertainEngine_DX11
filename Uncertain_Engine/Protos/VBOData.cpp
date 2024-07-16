#include "VBOData.h"

VBOData::VBOData()
	: TargetType(VBO_TARGET::DEFAULT), ComponentType(VBO_COMPONENT::DEFAULT),
	VboType(VBO_TYPE::DEFAULT), AttribIndex(0), Count(0), DataSize(0)
{
}

VBOData::VBOData(const VBOData& v)
{
	this->TargetType = v.TargetType;
	this->ComponentType = v.ComponentType;
	this->VboType = v.VboType;
	this->AttribIndex = v.AttribIndex;
	this->Count = v.Count;
	this->DataSize = v.DataSize;
	this->bIsEnabled = v.bIsEnabled;

	if (this->poData)
	{
		delete[]this->poData;
	}

	this->poData = new unsigned char[this->DataSize];
	memcpy_s(this->poData, this->DataSize, v.poData, v.DataSize);
}

VBOData& VBOData::operator=(const VBOData& v)
{
	this->TargetType = v.TargetType;
	this->ComponentType = v.ComponentType;
	this->VboType = v.VboType;
	this->AttribIndex = v.AttribIndex;
	this->Count = v.Count;
	this->bIsEnabled = v.bIsEnabled;
	this->DataSize = v.DataSize;

	if (this->poData)
	{
		delete[]this->poData;
	}

	this->poData = new unsigned char[this->DataSize];
	memcpy_s(this->poData, this->DataSize, v.poData, v.DataSize);

	return *this;
}

VBOData::~VBOData()
{
	if (this->poData)
	{
		delete[] this->poData;
		this->poData = nullptr;
	}
}

void VBOData::Serialize(VBOData_proto& out) const
{
	out.set_targettype((VBOData_proto_VBO_TARGET)this->TargetType);
	out.set_componenttype((VBOData_proto_VBO_COMPONENT)this->ComponentType);
	out.set_vbotype((VBOData_proto_VBO_TYPE)this->VboType);
	out.set_attribindex(this->AttribIndex);
	out.set_count(this->Count);
	out.set_datasize(this->DataSize);
	out.set_isenabled(this->bIsEnabled);

	std::string data((const char*)this->poData, this->DataSize);
	out.set_podata(data);
}

void VBOData::Deserialize(const VBOData_proto& in)
{
	this->TargetType = (VBO_TARGET)in.targettype();
	this->ComponentType = (VBO_COMPONENT)in.componenttype();
	this->VboType = (VBO_TYPE)in.vbotype();
	this->AttribIndex = in.attribindex();
	this->Count = in.count();
	this->DataSize = in.datasize();
	this->bIsEnabled = (bool)in.isenabled();

	if (this->poData)
	{
		delete[]this->poData;
	}

	this->poData = new unsigned char[this->DataSize];
	memcpy_s(this->poData, this->DataSize, in.podata().data(), in.datasize());
}

void VBOData::ConvertData(VBOData::VBO_COMPONENT componentReq, const unsigned char* pIndices, unsigned int stride)
{
	switch (componentReq)
	{
	case VBOData::VBO_COMPONENT::UNSIGNED_INT:
		ConverttoUInt(pIndices, stride);
		break;

	case VBOData::VBO_COMPONENT::BYTE:
	case VBOData::VBO_COMPONENT::UNSIGNED_BYTE:
	case VBOData::VBO_COMPONENT::SHORT:
	case VBOData::VBO_COMPONENT::UNSIGNED_SHORT:
	case VBOData::VBO_COMPONENT::INT:
	case VBOData::VBO_COMPONENT::FLOAT:
	case VBOData::VBO_COMPONENT::DOUBLE:
	case VBOData::VBO_COMPONENT::UNINITIALIZED:
	default:
		// Conversion type not implemented
		assert(false);
		break;
	}
}

void VBOData::ConverttoUInt(const unsigned char* pIndices, unsigned int stride)
{
	switch (ComponentType)
	{
	case VBOData::VBO_COMPONENT::UNSIGNED_INT:
		break;
	case VBOData::VBO_COMPONENT::BYTE:
		ConvertData_BytetoUInt(pIndices, stride);
		break;
	case VBOData::VBO_COMPONENT::UNSIGNED_SHORT:
		ConvertData_UShorttoUInt(pIndices, stride);
		break;

	case VBOData::VBO_COMPONENT::SHORT:
	case VBOData::VBO_COMPONENT::INT:
	case VBOData::VBO_COMPONENT::UNSIGNED_BYTE:
	case VBOData::VBO_COMPONENT::FLOAT:
	case VBOData::VBO_COMPONENT::DOUBLE:
	case VBOData::VBO_COMPONENT::UNINITIALIZED:
	default:
		// Conversion type not implemented
		assert(false);
		break;
	}
}

void VBOData::ConvertData_UShorttoUInt(const unsigned char* pIndices, unsigned int stride)
{
	assert(this->ComponentType == VBOData::VBO_COMPONENT::UNSIGNED_SHORT);
	unsigned int elementCount = this->Count * stride;

	unsigned int* pComponentBuffer = new unsigned int[elementCount]();
	unsigned int* pTemp = pComponentBuffer;
	unsigned short* pShort = (unsigned short*)pIndices;

	for (unsigned int i = 0; i < elementCount; i++)
	{
		
		*pTemp++ = *pShort++;
	}

	this->poData = (unsigned char*)pComponentBuffer;
	this->DataSize = elementCount * sizeof(unsigned int);
	this->ComponentType = VBOData::VBO_COMPONENT::UNSIGNED_INT;
}

void VBOData::ConvertData_BytetoUInt(const unsigned char* pIndices, unsigned int stride)
{
	assert(this->ComponentType == VBOData::VBO_COMPONENT::BYTE);
	unsigned int elementCount = this->Count * stride;

	unsigned int* pComponentBuffer = new unsigned int[elementCount]();
	unsigned int* pTemp = pComponentBuffer;

	for (unsigned int i = 0; i < elementCount; i++)
	{
		*pTemp = unsigned int(pIndices[i]);

		pTemp++;
	}

	this->poData = (unsigned char*)pComponentBuffer;
	this->DataSize = elementCount * sizeof(unsigned int);
	this->ComponentType = VBOData::VBO_COMPONENT::UNSIGNED_INT;
}

void VBOData::Print(const char* pName) const
{
	Trace::out("%s: \n", pName);
	Trace::out("       dataSize: %d \n", this->DataSize);
	float* pX = (float*)&poData[0];
	float* pY = &pX[1];
	float* pZ = &pX[2];
	Trace::out(" poBuff: %f %f %f \n", *pX, *pY, *pZ);
	Trace::out("    attribIndex: %d\n", this->AttribIndex);
	Trace::out("          count: %d \n", this->Count);
	Trace::out("  componentType: %d\n", this->ComponentType);
	Trace::out("        vboType: %d \n", this->VboType);
	Trace::out("     targetType: %d \n", this->TargetType);
}