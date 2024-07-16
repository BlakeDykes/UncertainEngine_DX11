#ifndef CONVERTER_ANIM_H
#define CONVERTER_ANIM_H

#include "Animdata.h"

class Converter_AnimData
{
public:
	static AnimData::PATH GetPath(std::string gltfPath);
	static AnimData::OUTPUT_TYPE GetOutputType(int GLTF_TINY_VAL);
	//static AnimData::COMPONENT_TYPE GetComponentType(int GLTF_TINY_VAL);

};

#endif // !CONVERTER_VBO_H
