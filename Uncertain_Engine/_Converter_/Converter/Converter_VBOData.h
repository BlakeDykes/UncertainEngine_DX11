#ifndef CONVERTER_VBO_H
#define CONVERTER_VBO_H

#include "VBOData.h"

class Converter_VBOData
{
public:
	static VBOData::VBO_TARGET GetTarget(int GLTF_TINY_VAL);
	static VBOData::VBO_COMPONENT GetComponent(int GLTF_TINY_VAL);
	static VBOData::VBO_TYPE GetType(int GLTF_TINY_VAL);
	static unsigned int GetStride(VBOData::VBO_TYPE type);

};

#endif // !CONVERTER_VBO_H
