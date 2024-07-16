#ifndef MODEL_TEXTURE_DATA_H
#define MODEL_TEXTURE_DATA_H

#include "TextureData.h"
#include "ModelTextureData.pb.h"

class ModelTextureData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;
	static const char* FILE_PREFIX;


	ModelTextureData();
	ModelTextureData(const ModelTextureData& t);
	const ModelTextureData& operator=(ModelTextureData& t);
	~ModelTextureData();

	void Serialize(ModelTextureData_proto& out) const;
	void Deserialize(ModelTextureData_proto& in);

	char pName[FILE_NAME_SIZE];
	unsigned int TextureCount;
	TextureData* Textures;
};


#endif	// !MODEL_TEXTURE_DATA_H