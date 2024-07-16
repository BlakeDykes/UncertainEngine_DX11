#include "TextureData.h"
#include "ModelTextureData.h"

const char* ModelTextureData::FILE_PREFIX = "t_all_";

ModelTextureData::ModelTextureData()
	:pName{ 0 }, TextureCount(0), Textures(nullptr)
{
}

ModelTextureData::ModelTextureData(const ModelTextureData& t)
{
	memcpy_s(this->pName, FILE_NAME_SIZE, t.pName, FILE_NAME_SIZE - 1);
	this->TextureCount = t.TextureCount;
	this->Textures = new TextureData[TextureCount]();

	for (unsigned int i = 0; i < TextureCount; i++)
	{
		this->Textures[i] = t.Textures[i];
	}
}

const ModelTextureData& ModelTextureData::operator=(ModelTextureData& t)
{
	if (this != &t)
	{
		memcpy_s(this->pName, FILE_NAME_SIZE, t.pName, FILE_NAME_SIZE - 1);
		this->TextureCount = t.TextureCount;
		this->Textures = new TextureData[TextureCount]();

		for (unsigned int i = 0; i < TextureCount; i++)
		{
			this->Textures[i] = t.Textures[i];
		}
	}

	return *this;
}

ModelTextureData::~ModelTextureData()
{
	if (this->Textures)
	{
		delete[] this->Textures;
		this->Textures = nullptr;
	}
}


void ModelTextureData::Serialize(ModelTextureData_proto& out) const
{
	std::string textName((const char*)this->pName, FILE_NAME_SIZE);
	out.set_name(textName);
	out.set_texturecount(this->TextureCount);

	TextureData* pText = this->Textures;
	TextureData_proto* textProto;

	for (unsigned int i = 0; i < this->TextureCount; i++)
	{
		textProto = out.add_textures();
		pText->Serialize(*textProto);
		pText++;
	}
}

void ModelTextureData::Deserialize(ModelTextureData_proto& in)
{
	this->TextureCount = in.texturecount();

	if (this->TextureCount > 0)
	{
		if (this->Textures)
		{
			delete[] this->Textures;
		}

		this->Textures = new TextureData[this->TextureCount]();
		TextureData_proto* textProto = nullptr;
		TextureData* pTexture = this->Textures;

		for (unsigned int i = 0; i < this->TextureCount; i++)
		{
			textProto = in.mutable_textures((int)i);
			pTexture->Deserialize(*textProto);
			pTexture++;
		}
	}

	memcpy_s(this->pName, FILE_NAME_SIZE, in.name().data(), FILE_NAME_SIZE);
}
