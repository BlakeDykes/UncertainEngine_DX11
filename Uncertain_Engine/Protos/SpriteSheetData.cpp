#include "SpriteSheetData.h"
#include "File.h"

using namespace Uncertain;

SpriteSheetData::SpriteSheetData()
	: pText(new TextureData()), ImageCount(0), pImages(nullptr)
{
}

SpriteSheetData::~SpriteSheetData()
{
	if (this->pImages)
	{
		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			delete this->pImages[i];
		}

		delete[] this->pImages;
	}

	delete this->pText;
}

SpriteSheetData::SpriteSheetData(TextureData::TEXTURE_TYPE textType, const char* textFilePath, 
									const char* xmlFilePath, const char* subTextureElement, TYPE spriteType)
	: Type(spriteType), pText(new TextureData()), ImageCount(0), pImages(nullptr)
{

	this->GetNameFromFilePath(textFilePath, this->Name);

	switch (textType)
	{
	case TextureData::TEXTURE_TYPE::PNG:
		*this->pText = TextureData::CreateTexturePNG(textFilePath);
		break;
	case TextureData::TEXTURE_TYPE::TGA:
		*this->pText = TextureData::CreateTextureTGA(textFilePath);
		break;

	case TextureData::TEXTURE_TYPE::JPEG:
	case TextureData::TEXTURE_TYPE::BMP:
	case TextureData::TEXTURE_TYPE::GIF:
	default:
		assert(false);
		break;
	}

	if (this->pText)
	{
		this->pText->bBaseColorEnabled = false;
		this->pText->bTextEnabled = true;
		this->pText->TextureIndex = 0;
		this->pText->MaterialIndex = 0;
	}

	this->GenerateImages(xmlFilePath, subTextureElement);
}


void SpriteSheetData::Serialize(SpriteSheetData_proto& out) const
{
	std::string name((const char*)this->Name, IMAGE_NAME_SIZE);
	
	out.set_name(name);
	out.set_imagecount(this->ImageCount);
	out.set_type((SpriteSheetData_proto_TYPE)this->Type);

	assert(this->pText);
	TextureData_proto* textProto = new TextureData_proto();
	this->pText->Serialize(*textProto);
	out.set_allocated_text(textProto);

	assert(this->pImages);
	switch (this->Type)
	{
	case SpriteSheetData::TYPE::SPRITE:
	{
		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			SpriteSheetData_proto_Image* image = out.add_images();
			std::string imgName((const char*)this->pImages[i]->ImageName, IMAGE_NAME_SIZE);
			image->set_imagename(imgName);
			image->set_height(this->pImages[i]->Height);
			image->set_width(this->pImages[i]->Width);
			image->set_y(this->pImages[i]->Y);
			image->set_x(this->pImages[i]->X);
		}

		break;
	}
	case SpriteSheetData::TYPE::FONT:
	{
		Font* pCur = nullptr;

		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			pCur = (Font*)this->pImages[i];
			SpriteSheetData_proto_Image* image = out.add_images();
			std::string imgName((const char*)pCur->ImageName, IMAGE_NAME_SIZE);
			image->set_imagename(imgName);
			image->set_height(pCur->Height);
			image->set_width(pCur->Width);
			image->set_y(pCur->Y);
			image->set_x(pCur->X);
			image->set_key(pCur->GetKey());
		}

		break;
	}
	default:
		assert(false);
		break;
	}

}

void SpriteSheetData::Deserialize(SpriteSheetData_proto& in)
{
	memcpy_s(this->Name, IMAGE_NAME_SIZE, in.name().data(), IMAGE_NAME_SIZE);
	this->Type = (SpriteSheetData::TYPE)in.type();
	this->ImageCount = in.imagecount();
	this->pText->Deserialize(in.text());

	if (this->pImages)
	{
		delete[] this->pImages;
	}

	this->pImages = new Image*[this->ImageCount];

	SpriteSheetData_proto_Image* pProtoImage = nullptr;

	switch (this->Type)
	{
	case SpriteSheetData::TYPE::SPRITE:
	{
		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			this->pImages[i] = new Image();
			pProtoImage = in.mutable_images(i);
			memcpy_s(this->pImages[i]->ImageName, IMAGE_NAME_SIZE, pProtoImage->imagename().data(), IMAGE_NAME_SIZE);
			this->pImages[i]->Height = pProtoImage->height();
			this->pImages[i]->Width = pProtoImage->width();
			this->pImages[i]->Y = pProtoImage->y();
			this->pImages[i]->X = pProtoImage->x();
		}

		break;
	}
	case SpriteSheetData::TYPE::FONT:
	{
		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			this->pImages[i] = new Font();

			pProtoImage = in.mutable_images(i);
			memcpy_s(this->pImages[i]->ImageName, IMAGE_NAME_SIZE, pProtoImage->imagename().data(), IMAGE_NAME_SIZE);
			this->pImages[i]->Height = pProtoImage->height();
			this->pImages[i]->Width = pProtoImage->width();
			this->pImages[i]->Y = pProtoImage->y();
			this->pImages[i]->X = pProtoImage->x();
			this->pImages[i]->SetKey(pProtoImage->key());
		}

		break;
	}
	default:
		assert(false);
		break;
	}
}

void SpriteSheetData::GetNameFromFilePath(const char* filePath, char* nameOut)
{
	unsigned int totalLength = strlen(filePath);

	// Strip the filepath and extension off the name
	unsigned int starting = totalLength;
	while (starting != 0 && filePath[starting] != '/')
	{
		starting--;
	}
	assert(starting != 0);
	unsigned int ending = starting;

	while (ending < totalLength && filePath[ending] != '.')
	{
		ending++;
	}

	unsigned int len = ending - starting;

	strncpy_s(nameOut, len, &filePath[starting + 1], _TRUNCATE);
}


void SpriteSheetData::GenerateImages(const char* xmlFilePath, const char* subTextureElement)
{
	assert(xmlFilePath);

	char* buffer = nullptr;
	DWORD fileSize;

	File::Error err;
	File::Handle fh;

	err = File::Open(fh, xmlFilePath, File::Mode::READ);
	assert(err == File::Error::SUCCESS);

	err = File::GetSize(fh, fileSize);
	assert(err == File::Error::SUCCESS);

	buffer = new char[fileSize]();
	assert(buffer);

	err = File::Read(fh, buffer, fileSize);
	assert(err == File::Error::SUCCESS);

	this->ImageCount = CountSubTextures(subTextureElement, buffer, fileSize);
	assert(this->ImageCount > 0);

	const char* ptr = buffer;

	this->pImages = new Image * [this->ImageCount];

	switch (this->Type)
	{
	case SpriteSheetData::TYPE::SPRITE:
	{
		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			this->pImages[i] = new Image();
			ptr = strstr(ptr, subTextureElement);
			ReadSubTextureData(this->pImages[i], ptr);
			ptr = strstr(ptr, "/");
		}

		break;
	}
	case SpriteSheetData::TYPE::FONT:
	{

		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			this->pImages[i] = new Font();
			ptr = strstr(ptr, subTextureElement);
			ReadSubTextureData_Font((Font*)this->pImages[i], ptr);

			ptr = strstr(ptr, "/");
		}
		break;
	}
	default:
		assert(false);
		break;
	}

	delete[] buffer;
}

unsigned int SpriteSheetData::CountSubTextures(const char* subTextureElement, const char* buffer, DWORD bufferSize)
{
	const char* ptr = buffer;
	const char* temp = nullptr;
	size_t length = strlen(subTextureElement);
	unsigned int count = 0;

	while (ptr != nullptr && (unsigned int)(ptr - buffer) != bufferSize)
	{
		temp = ptr + 1;
		if (*ptr == subTextureElement[0])
		{
			size_t i = 0;
			while (i < length && ptr[i] == subTextureElement[i])
			{
				i++;
			}
			if (i == length)
			{
				count++;
				temp = &ptr[i];
			}
		}
		
		ptr = temp;
	}

	return count;
}

void SpriteSheetData::ReadSubTextureData(Image* pImage, const char* buffer)
{
	const char* name = " name=\"";
	const char* height = " height=\"";
	const char* width = " width=\"";
	const char* y = " y=\"";
	const char* x = " x=\"";

	GetAttribData(buffer, name, &pImage->ImageName[0]);
	GetAttribData(buffer, height, pImage->Height);
	GetAttribData(buffer, width, pImage->Width);
	GetAttribData(buffer, y, pImage->Y);
	GetAttribData(buffer, x, pImage->X);
}

void SpriteSheetData::ReadSubTextureData_Font(Font* pFont, const char* buffer)
{
	const char* name = " name=\"";
	const char* key = " key=\"";
	const char* height = " height=\"";
	const char* width = " width=\"";
	const char* y = " y=\"";
	const char* x = " x=\"";

	GetAttribData(buffer, name, &pFont->ImageName[0]);
	GetAttribData(buffer, key, pFont->GetKey());
	GetAttribData(buffer, height, pFont->Height);
	GetAttribData(buffer, width, pFont->Width);
	GetAttribData(buffer, y, pFont->Y);
	GetAttribData(buffer, x, pFont->X);
}


void SpriteSheetData::GetAttribData(const char* buffer, const char* attribName, char* out)
{
	const char* ptr = strstr(buffer, attribName);
	ptr += strlen(attribName);

	int i = 0;

	while (i < IMAGE_NAME_SIZE - 1 && ptr[i] != '"')
	{
		out[i] = ptr[i++];
	}
}

void SpriteSheetData::GetAttribData(const char* buffer, const char* attribName, int& out)
{
	const char* ptr = strstr(buffer, attribName);
	ptr += strlen(attribName);

	char strNum[10];
	int i = 0;

	while (isdigit(ptr[i]))
	{
		assert(i < 10);
		strNum[i] = ptr[i++];
	}

	out = atoi(strNum);
}

void SpriteSheetData::PrintAll()
{
	Trace::out("- SpriteSheet - %s\n", this->Name);
	Trace::out("- Type - %d\n", this->Type);
	Trace::out("---	Texture - %s\n", this->pText->pFileName);
	Trace::out("--- ImageCount - %d\n", this->ImageCount);


	switch (this->Type)
	{
	case SpriteSheetData::TYPE::SPRITE:
	{
		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			Trace::out("--- Image - %s\n", this->pImages[i]->ImageName);
			Trace::out("-------- 	  X - %d\n", this->pImages[i]->X);
			Trace::out("-------- 	  Y - %d\n", this->pImages[i]->Y);
			Trace::out("-------- Width  - %d\n", this->pImages[i]->Width);
			Trace::out("-------- Height - %d\n", this->pImages[i]->Height);
		}
		break;
	}
	case SpriteSheetData::TYPE::FONT:
	{
		Font* pCur;

		for (unsigned int i = 0; i < this->ImageCount; i++)
		{
			pCur = (Font*)this->pImages[i];
			Trace::out("--- Image - %s\n",		 pCur->ImageName);
			Trace::out("-------- 	key - %d\n", pCur->GetKey());
			Trace::out("-------- 	  X - %d\n", pCur->X);
			Trace::out("-------- 	  Y - %d\n", pCur->Y);
			Trace::out("-------- Width  - %d\n", pCur->Width);
			Trace::out("-------- Height - %d\n", pCur->Height);
		}
		break;
	}
	default:
		assert(false);
		break;
	}
}

void SpriteSheetData::Image::SetKey(const int key)
{
	AZUL_UNUSED_VAR(key);
	assert(false);
}

int& SpriteSheetData::Image::GetKey()
{
	assert(false);
	int* shutUpCompiler = new int();
	return *shutUpCompiler;
}

void SpriteSheetData::Font::SetKey(const int key)
{
	this->Key = key;
}

int& SpriteSheetData::Font::GetKey()
{
	return this->Key;
}
