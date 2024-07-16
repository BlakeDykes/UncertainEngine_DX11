#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H


#include "TextureData.pb.h"

class TextureData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;
	static const char* FILE_PREFIX;

	enum class TEXTURE_TYPE
	{
		JPEG = 0,
		PNG = 1,
		BMP = 2,
		GIF = 3,
		TGA = 4,
		DEFAULT = PNG
	};

	enum class TEXTURE_MAG_FILTER
	{
		MAG_NEAREST = 0,
		MAG_LINEAR = 1,
		DEFAULT = MAG_LINEAR
	};

	enum class TEXTURE_MIN_FILTER
	{
		MIN_NEAREST = 0,
		MIN_LINEAR = 1,
		MIN_NEAREST_MIPMAP_NEAREST = 2,
		MIN_LINEAR_MIPMAP_NEAREST = 3,
		MIN_NEAREST_MIPMAP_LINEAR = 4,
		MIN_LINEAR_MIPMAP_LINEAR = 5,
		DEFAULT = MIN_LINEAR
	};

	enum class TEXTURE_WRAP
	{
		WRAP_CLAMP_TO_EDGE = 0,
		WRAP_MIRRORED_REPEAT = 1,
		WRAP_REPEAT = 2,
		DEFAULT = WRAP_CLAMP_TO_EDGE
	};

	enum class TEXTURE_COMPONENT
	{
		BYTE = 0,
		UNSIGNED_BYTE = 1,
		SHORT = 2,
		UNSIGNED_SHORT = 3,
		INT = 4,
		UNSIGNED_INT = 5,
		FLOAT = 6,
		DOUBLE = 7,
		DEFAULT = UNSIGNED_BYTE
	};

	enum class TEXTURE_EFORMAT
	{
		EFORMAT_BGR = 0,
		EFORMAT_RGB = 1,
		EFORMAT_RGBA = 2,
		EFORMAT_BGRA = 3,
		DEFAULT = EFORMAT_RGBA
	};

	enum class TEXTURE_NCOMPONENT
	{
		NCOMPONENT_BGR = 0,
		NCOMPONENT_RGB = 1,
		NCOMPONENT_RGBA = 2,
		NCOMPONENT_BGRA = 3,
		DEFAULT = NCOMPONENT_RGBA
	};

	struct BaseColorFactor
	{
		float r;
		float g;
		float b;
		float a;

		BaseColorFactor();
		const BaseColorFactor& operator=(const BaseColorFactor& b);
		BaseColorFactor(const BaseColorFactor& b);
		~BaseColorFactor() = default;

		void Set(const TextureData_proto::BaseColorFactor& b);
		void Set(float _r, float _g, float _b, float _a);
	};

	TextureData();
	TextureData(const TextureData& t);
	TextureData& operator=(const TextureData& t);
	~TextureData();

	static TextureData CreateTexturePNG(const char* const pFileName);
	static TextureData CreateTextureTGA(const char* const pFileName);
	static bool ReadPNGBits(const char* const pFileName, TextureData& textOut);
	static bool ReadTGABits(const char* const pFileName, TextureData& textOut);

	void Serialize(TextureData_proto& out) const;
	void Deserialize(const TextureData_proto& in);

	void Print() const;

	char                pFileName[FILE_NAME_SIZE];
	BaseColorFactor		BaseColor;
	TEXTURE_TYPE        TextType;
	TEXTURE_MAG_FILTER  MagFilter;
	TEXTURE_MIN_FILTER  MinFilter;
	TEXTURE_WRAP        WrapS;
	TEXTURE_WRAP        WrapT;

	TEXTURE_EFORMAT		EFormat;
	TEXTURE_NCOMPONENT	NComponent;

	unsigned int        Width;
	unsigned int        Height;
	unsigned int        Component;  // bits per channel
	unsigned int        Bits;       // bits per channel 8, 16, 32 per color
	TEXTURE_COMPONENT	PixelType;

	unsigned int		MaterialIndex;
	unsigned int		TextureIndex;
	unsigned int        DataSize;
	bool                AsIs;
	bool				bTextEnabled;
	bool				bBaseColorEnabled;
	unsigned char*		poData;
};


#endif