#ifndef	SPRITESHEET_DATA_H
#define SPRITESHEET_DATA_H

#include "TextureData.h"
#include "SpriteSheetData.pb.h"

class SpriteSheetData
{
public:
	static const unsigned int IMAGE_NAME_SIZE = 16;

	enum class TYPE
	{
		SPRITE = 0,
		FONT = 1
	};

	struct Image
	{
		char ImageName[IMAGE_NAME_SIZE];
		int Height;
		int Width;
		int Y;
		int X;

		virtual void SetKey(const int key);
		virtual int& GetKey();

	};

	struct Font : public Image
	{
		int Key;
		void SetKey(const int key) override final;
		int& GetKey() override final;
	};

	SpriteSheetData();
	SpriteSheetData(const SpriteSheetData& t) = delete;
	SpriteSheetData& operator=(const SpriteSheetData& t) = delete;
	~SpriteSheetData();

	SpriteSheetData(TextureData::TEXTURE_TYPE textType, const char* textFilePath, const char* xmlFilePath, const char* subTextureElement, TYPE spriteType);

	void Serialize(SpriteSheetData_proto& out) const;
	void Deserialize(SpriteSheetData_proto& in);

	void PrintAll();

	char Name[IMAGE_NAME_SIZE];
	TYPE Type;
	TextureData* pText;
	unsigned int ImageCount;
	Image** pImages;
	
private:
	void GetNameFromFilePath(const char* filePath, char* nameOut);
	void GenerateImages(const char* filePath, const char* subTextureElement);
	unsigned int CountSubTextures(const char* subTextureElement, const char* buffer, DWORD bufferSize);
	void ReadSubTextureData(Image* pImage, const char* buffer);
	void ReadSubTextureData_Font(Font* pImage, const char* buffer);
	void GetAttribData(const char* buffer, const char* attribName, char* out);
	void GetAttribData(const char* buffer, const char* attribName, int& out);
};


#endif