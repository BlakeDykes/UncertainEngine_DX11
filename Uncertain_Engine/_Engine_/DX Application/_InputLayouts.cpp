#include "_InputLayouts.h"

ShaderCompileHelper GetInputLayout(ShaderName name)
{
	switch (name)
	{
	case EngineNames::ShaderName::ColorLight:			return { GetFileName(name), IA_COLOR_LIGHT,		ARRAYSIZE(IA_COLOR_LIGHT) };
	case EngineNames::ShaderName::ColorLight_1_3_1:		return { GetFileName(name), IA_COLOR_LIGHT,		ARRAYSIZE(IA_COLOR_LIGHT) };
	case EngineNames::ShaderName::SimpleColor3D:		return { GetFileName(name), IA_SIMPLE_COLOR,	ARRAYSIZE(IA_SIMPLE_COLOR) };
	case EngineNames::ShaderName::ColorSelected3D:		return { GetFileName(name), IA_COLOR_SELECTED,	ARRAYSIZE(IA_COLOR_SELECTED) };
	case EngineNames::ShaderName::SimpleTexture:		return { GetFileName(name), IA_SIMPLE_TEXTURE,	ARRAYSIZE(IA_SIMPLE_TEXTURE) };
	case EngineNames::ShaderName::TextureLight:			return { GetFileName(name), IA_TEXTURE_LIGHT,	ARRAYSIZE(IA_TEXTURE_LIGHT) };
	case EngineNames::ShaderName::TextureLight_1_3_1:	return { GetFileName(name), IA_TEXTURE_LIGHT,	ARRAYSIZE(IA_TEXTURE_LIGHT) };
	case EngineNames::ShaderName::BlendedTile:			return { GetFileName(name), IA_BLENDED_TILE,	ARRAYSIZE(IA_BLENDED_TILE) };
	case EngineNames::ShaderName::BlendedTile_1_3_1:	return { GetFileName(name), IA_BLENDED_TILE,	ARRAYSIZE(IA_BLENDED_TILE) };
	case EngineNames::ShaderName::SpectrumLight:		return { GetFileName(name), IA_SPECTRUM_LIGHT,	ARRAYSIZE(IA_SPECTRUM_LIGHT) };
	case EngineNames::ShaderName::MagnitudeLight:		return { GetFileName(name), IA_MAGNITUDE_LIGHT,	ARRAYSIZE(IA_MAGNITUDE_LIGHT) };

	case EngineNames::ShaderName::UNINITIALIZED:
	default:
		assert(false);
		return ShaderCompileHelper();
	}
}