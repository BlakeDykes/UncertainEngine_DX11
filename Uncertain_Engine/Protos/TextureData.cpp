#include "TextureData.h"
#include "File.h"

#ifndef CONVERTER

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#endif

#include "stb_image.h"

using namespace Uncertain;

const char* TextureData::FILE_PREFIX = "t_";

// Define targa header. This is only used locally.
#pragma pack(1)
typedef struct
{
	char	identsize;               // Size of ID field that follows header (0)
	char	colorMapType;            // 0 = None, 1 = paletted
	char	imageType;               // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;   // First colour map entry
	unsigned short	colorMapLength;  // Number of colors
	unsigned char 	colorMapBits;    // bits per palette entry
	unsigned short	xstart;          // image x origin
	unsigned short	ystart;          // image y origin
	unsigned short	width;           // width in pixels
	unsigned short	height;          // height in pixels
	char	bits;                    // bits per pixel (8 16, 24, 32)
	char	descriptor;              // image descriptor
} TGAHEADER;
#pragma pack(8)

TextureData::TextureData()
	: pFileName{ 0x0 }, BaseColor(), TextType(TEXTURE_TYPE::DEFAULT), MagFilter(TEXTURE_MAG_FILTER::DEFAULT),
	MinFilter(TEXTURE_MIN_FILTER::DEFAULT),	WrapS(TEXTURE_WRAP::DEFAULT), WrapT(TEXTURE_WRAP::DEFAULT), 
	EFormat(TEXTURE_EFORMAT::DEFAULT), NComponent(TEXTURE_NCOMPONENT::DEFAULT),
	Width(0), Height(0), Component(0), Bits(0), PixelType(TEXTURE_COMPONENT::DEFAULT), MaterialIndex(UINT_MAX),
	TextureIndex(UINT_MAX), DataSize(0), AsIs(false), bTextEnabled(false), bBaseColorEnabled(false), poData(nullptr)
{
}

TextureData::TextureData(const TextureData& t)
{
	memcpy_s(this->pFileName, FILE_NAME_SIZE, t.pFileName, FILE_NAME_SIZE - 1);

	this->BaseColor				= t.BaseColor;
	this->TextType				= t.TextType;
	this->MagFilter				= t.MagFilter;
	this->MinFilter				= t.MinFilter;
	this->WrapS					= t.WrapS;
	this->WrapT					= t.WrapT;
	this->EFormat				= t.EFormat;
	this->NComponent			= t.NComponent;
	this->Width					= t.Width;
	this->Height				= t.Height;
	this->Component				= t.Component;
	this->Bits					= t.Bits;
	this->PixelType				= t.PixelType;
	this->MaterialIndex			= t.MaterialIndex;
	this->TextureIndex			= t.TextureIndex;
	this->DataSize				= t.DataSize;
	this->AsIs					= t.AsIs;
	this->bTextEnabled			= t.bTextEnabled;
	this->bBaseColorEnabled		= t.bBaseColorEnabled;

	this->poData = new unsigned char[t.DataSize]();
	memcpy_s(this->poData, this->DataSize, t.poData, t.DataSize);
}

TextureData& TextureData::operator = (const TextureData& t)
{
	if (this != &t)
	{
		memcpy_s(this->pFileName, FILE_NAME_SIZE, t.pFileName, FILE_NAME_SIZE - 1);
		
		this->BaseColor = t.BaseColor;
		this->TextType			= t.TextType;
		this->MagFilter			= t.MagFilter;
		this->MinFilter			= t.MinFilter;
		this->WrapS				= t.WrapS;
		this->WrapT				= t.WrapT;
		this->EFormat			= t.EFormat;
		this->NComponent		= t.NComponent;
		this->Width				= t.Width;
		this->Height			= t.Height;
		this->Component			= t.Component;
		this->Bits				= t.Bits;
		this->PixelType			= t.PixelType;
		this->MaterialIndex		= t.MaterialIndex;
		this->TextureIndex		= t.TextureIndex;
		this->DataSize			= t.DataSize;
		this->AsIs				= t.AsIs;
		this->bTextEnabled		= t.bTextEnabled;
		this->bBaseColorEnabled = t.bBaseColorEnabled;


		if (this->poData != nullptr)
		{
			delete[] this->poData;
		}

		this->poData = new unsigned char[t.DataSize]();
		memcpy_s(this->poData, this->DataSize, t.poData, t.DataSize);
	}

	return *this;
}

TextureData::~TextureData()
{
	if (this->poData)
	{
		delete[] this->poData;
		this->poData = nullptr;
	}
}

void TextureData::Serialize(TextureData_proto& out) const
{
	assert(bTextEnabled || bBaseColorEnabled);

	out.set_filenamesize(FILE_NAME_SIZE);
	std::string s0((const char*)this->pFileName, FILE_NAME_SIZE);
	out.set_filenamedata(s0);

	out.set_textenabled(this->bTextEnabled);
	out.set_basecolorenabled(this->bBaseColorEnabled);
	out.set_materialindex(this->MaterialIndex);
	out.set_textureindex(this->TextureIndex);

	if (bBaseColorEnabled)
	{
		TextureData_proto_BaseColorFactor* pBaseColorProto = out.mutable_basecolor();
		pBaseColorProto->set_r(this->BaseColor.r);
		pBaseColorProto->set_g(this->BaseColor.g);
		pBaseColorProto->set_b(this->BaseColor.b);
		pBaseColorProto->set_a(this->BaseColor.a);
	}
	if (bTextEnabled)
	{
		out.set_texttype((TextureData_proto_TEXTURE_TYPE)this->TextType);
		out.set_magfilter((TextureData_proto_TEXTURE_MAG_FILTER)this->MagFilter);
		out.set_minfilter((TextureData_proto_TEXTURE_MIN_FILTER)this->MinFilter);
		out.set_wraps((TextureData_proto_TEXTURE_WRAP)this->WrapS);
		out.set_wrapt((TextureData_proto_TEXTURE_WRAP)this->WrapT);
		out.set_eformat((TextureData_proto_TEXTURE_EFORMAT)this->EFormat);
		out.set_ncomponent((TextureData_proto_TEXTURE_NCOMPONENT)this->NComponent);
		out.set_width(this->Width);
		out.set_height(this->Height);
		out.set_component(this->Component);
		out.set_bits(this->Bits);
		out.set_pixel_type((TextureData_proto_TEXTURE_COMPONENT)this->PixelType);
		out.set_as_is(this->AsIs);
		out.set_datasize(this->DataSize);

		std::string s1((const char*)this->poData, this->DataSize);
		out.set_podata(s1);
	}

}

void TextureData::Deserialize(const TextureData_proto& in)
{
	memcpy_s(this->pFileName, FILE_NAME_SIZE, in.filenamedata().data(), in.filenamesize());

	this->bTextEnabled = (bool)in.textenabled();
	this->bBaseColorEnabled = (bool)in.basecolorenabled();
	this->TextureIndex = in.textureindex();
	this->MaterialIndex = in.materialindex();

	if (bBaseColorEnabled)
	{
		this->BaseColor.Set(in.basecolor());
	}
	if (bTextEnabled)
	{
		this->TextType = (TEXTURE_TYPE)in.texttype();
		this->MagFilter = (TEXTURE_MAG_FILTER)in.magfilter();
		this->MinFilter = (TEXTURE_MIN_FILTER)in.minfilter();
		this->WrapS = (TEXTURE_WRAP)in.wraps();
		this->WrapT = (TEXTURE_WRAP)in.wrapt();
		this->EFormat = (TEXTURE_EFORMAT)in.eformat();
		this->NComponent = (TEXTURE_NCOMPONENT)in.ncomponent();
		this->Width = in.width();
		this->Height = in.height();
		this->Component = in.component();
		this->Bits = in.bits();
		this->PixelType = (TEXTURE_COMPONENT)in.pixel_type();
		this->AsIs = (bool)in.as_is();
		this->DataSize = in.datasize();

		this->poData = new unsigned char[this->DataSize]();
		memcpy_s(this->poData, this->DataSize, in.podata().data(), in.datasize());
	}
	
}


TextureData TextureData::CreateTexturePNG(const char* const pFileName)
{
	assert(pFileName);

	// Strip the filepath and extension off the name
	unsigned int starting = strlen(pFileName);
	while (starting != 0 && pFileName[starting] != '/') {
		starting--;
	}
	assert(starting != 0);

	unsigned int len = strlen(pFileName) - (starting+1);
	char* pTmp = new char[len + 1]();

	strncpy_s(pTmp, len + 1, &pFileName[starting + 1], _TRUNCATE);

	// runtime model
	TextureData text;

	text.TextType = TextureData::TEXTURE_TYPE::PNG;
	text.MagFilter = TextureData::TEXTURE_MAG_FILTER::MAG_LINEAR;
	text.MinFilter = TextureData::TEXTURE_MIN_FILTER::MIN_LINEAR;
	text.WrapS = TextureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
	text.WrapT = TextureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
	text.PixelType = TextureData::TEXTURE_COMPONENT::UNSIGNED_BYTE;
	text.AsIs = false;
	memcpy_s(text.pFileName, TextureData::FILE_NAME_SIZE, pTmp, len);

	delete[] pTmp;

	if (ReadPNGBits(pFileName, text)) 
	{
		text.bTextEnabled = true;
		return text;
	}
	else
	{
		assert(false);
		return text;
	}
}

TextureData TextureData::CreateTextureTGA(const char* const pFileName)
{
	assert(pFileName);

	// Strip the filepath and extension off the name
	unsigned int starting = strlen(pFileName);
	while (starting != 0 && pFileName[starting] != '/') {
		starting--;
	}
	assert(starting != 0);

	unsigned int len = strlen(pFileName) - (starting + 1);
	char* pTmp = new char[len + 1]();

	strncpy_s(pTmp, len + 1, &pFileName[starting + 1], _TRUNCATE);

	// runtime model
	TextureData text;

	text.TextType = TextureData::TEXTURE_TYPE::TGA;
	text.MagFilter = TextureData::TEXTURE_MAG_FILTER::MAG_LINEAR;
	text.MinFilter = TextureData::TEXTURE_MIN_FILTER::MIN_LINEAR;
	text.WrapS = TextureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
	text.WrapT = TextureData::TEXTURE_WRAP::WRAP_CLAMP_TO_EDGE;
	text.PixelType = TextureData::TEXTURE_COMPONENT::UNSIGNED_BYTE;
	text.AsIs = false;
	memcpy_s(text.pFileName, TextureData::FILE_NAME_SIZE, pTmp, len);

	delete[] pTmp;

	if (ReadTGABits(pFileName, text)) 
	{
		text.bTextEnabled = true;
		return text;
	}
	else
	{
		assert(false);
		return text;
	}
}

bool TextureData::ReadPNGBits(const char* const pFileName, TextureData& textOut)
{
	unsigned long ImageSize;   // Size in bytes of image
	unsigned char* pBits = nullptr;      // Pointer to bits

	// Default/Failed values
	int Width = 0;
	int Height = 0;
	int comp;
	int req_comp = 4;

	TextureData::TEXTURE_EFORMAT eFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_RGB;
	TextureData::TEXTURE_NCOMPONENT nComponents = TextureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;

	pBits = stbi_load(pFileName, &Width, &Height, &comp, req_comp);
	if (pBits == nullptr)
	{
		return false;
	}

	ImageSize = (unsigned int)(Width * comp * Height);

	// Requesting RGBA, fallback to RGB
	switch (comp)
	{
	case 4:
		eFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_BGRA;
		nComponents = TextureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
		break;

	case 3:   
		eFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_RGB;
		nComponents = TextureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;
		break;

	case 2:
	case 1:
		assert(false);
		return false;
		break;

	default:       

		break;
	}

	textOut.EFormat = eFormat;
	textOut.NComponent = nComponents;
	textOut.Width = (unsigned int)Width;
	textOut.Height = (unsigned int)Height;
	textOut.Component = (unsigned int)req_comp;
	textOut.Bits = (unsigned int)8 * comp;
	textOut.DataSize = ImageSize;
	textOut.poData = (unsigned char*)pBits;

	return true;
}

bool TextureData::ReadTGABits(const char* const pFileName, TextureData& textOut)
{

	// Default values

	int width = 0;
	int height = 0;
	int comp;
	int req_comp = 4;
	unsigned char* pBits = nullptr;

	pBits = stbi_load(pFileName, &width, &height, &comp, req_comp);
	if (pBits == nullptr) 
	{
		return false;
	}

	TEXTURE_EFORMAT eFormat = TEXTURE_EFORMAT::EFORMAT_RGB;
	TEXTURE_NCOMPONENT nComponents = TEXTURE_NCOMPONENT::NCOMPONENT_RGB;
	unsigned long imageSize = (unsigned int)(width * height * req_comp);

	// Set OpenGL format expected
	// Requesting RGBA, fallback to RGB
	switch (comp)
	{
	case 4:
		eFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_BGRA;
		nComponents = TextureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGBA;
		break;
	case 3:     
		eFormat = TextureData::TEXTURE_EFORMAT::EFORMAT_RGB;
		nComponents = TextureData::TEXTURE_NCOMPONENT::NCOMPONENT_RGB;
		break;
	case 2:
	case 1:
		assert(false);
		return false;
		break;

	default:

		break;
	}

	textOut.EFormat = eFormat;
	textOut.NComponent = nComponents;
	textOut.Width = (unsigned int)width;
	textOut.Height = (unsigned int)height;
	textOut.Component = (unsigned int)req_comp;
	textOut.Bits = (unsigned int)comp;
	textOut.DataSize = imageSize;
	textOut.poData = (unsigned char*)pBits;

	return true;
}

void TextureData::Print() const
{
	Trace::out("%s: \n", this->pFileName);
	Trace::out("      dataSize: %d \n", this->DataSize);
	Trace::out("        poData: %2x %2x %2x %2x %2x \n",
		this->poData[0],
		this->poData[1],
		this->poData[2],
		this->poData[3],
		this->poData[4]);
	Trace::out("      textType: %d\n", this->TextType);
	Trace::out("     magFilter: %d \n", this->MagFilter);
	Trace::out("     minFilter: %d\n", this->MinFilter);
	Trace::out("         wrapS: %d \n", this->WrapS);
	Trace::out("         wrapT: %d \n", this->WrapT);
	Trace::out("         width: %d \n", this->Width);
	Trace::out("        height: %d \n", this->Height);
}

TextureData::BaseColorFactor::BaseColorFactor()
	:r(1.0f), g(1.0f), b(1.0f), a(1.0f)
{
}

const TextureData::BaseColorFactor& TextureData::BaseColorFactor::operator=(const BaseColorFactor& baseColor)
{
	if (this != &baseColor)
	{
		this->r = baseColor.r;
		this->g = baseColor.g;
		this->b = baseColor.b;
		this->a = baseColor.a;
	}

	return *this;
}

TextureData::BaseColorFactor::BaseColorFactor(const BaseColorFactor& baseColor)
{
	this->r = baseColor.r;
	this->g = baseColor.g;
	this->b = baseColor.b;
	this->a = baseColor.a;
}

void TextureData::BaseColorFactor::Set(const TextureData_proto::BaseColorFactor& baseColor)
{
	this->r = baseColor.r();
	this->g = baseColor.g();
	this->b = baseColor.b();
	this->a = baseColor.a();
}

void TextureData::BaseColorFactor::Set(float _r, float _g, float _b, float _a)
{
	this->r = _r;
	this->g = _g;
	this->b = _b;
	this->a = _a;
}
