#include <d3d11.h>
#include "d3dUtil.h"
#include "Model.h"
#include "EngineNames.h"

struct ShaderCompileHelper
{
	WCHAR* fn;
	D3D11_INPUT_ELEMENT_DESC* ia;
	size_t count;

	ShaderCompileHelper() = default;

	ShaderCompileHelper(WCHAR* _fn, D3D11_INPUT_ELEMENT_DESC* _ia, size_t _count )
		: fn(_fn), ia(_ia), count(_count)
	{ }

};

// ----------------------------------------------------------
//  Defaults ------------------------------------------------
// ----------------------------------------------------------
static const char* NAME_POS = "POSITION";
static const char* NAME_UV = "TEXCOORD";
static const char* NAME_NORMAL = "NORMAL";
static const char* NAME_COLOR = "COLOR";
static const char* NAME_BLEND  = "BLEND";
static const char* NAME_SPECMOD   = "SPECMOD";

static const D3D11_INPUT_ELEMENT_DESC DEFAULT_POS_VERT_DATA =
{
	NAME_POS,
	0,
	FORMAT_POS,
	0,
	OFFSET_POS,
	D3D11_INPUT_PER_VERTEX_DATA,
	0
};

static const D3D11_INPUT_ELEMENT_DESC DEFAULT_UV_VERT_DATA =
{
	NAME_UV,
	0,
	FORMAT_UV,
	0,
	OFFSET_UV,
	D3D11_INPUT_PER_VERTEX_DATA,
	0
};

static const D3D11_INPUT_ELEMENT_DESC DEFAULT_NORMAL_VERT_DATA =
{
	NAME_NORMAL,
	0,
	FORMAT_NORMAL,
	0,
	OFFSET_NORMAL,
	D3D11_INPUT_PER_VERTEX_DATA,
	0
};

static const D3D11_INPUT_ELEMENT_DESC DEFAULT_COLOR_VERT_DATA =
{
	NAME_COLOR,
	0,
	FORMAT_COLOR,
	0,
	OFFSET_COLOR,
	D3D11_INPUT_PER_VERTEX_DATA,
	0
};

static D3D11_INPUT_ELEMENT_DESC DEFAULT_BLEND_WEIGHT_VERT_DATA =
{
	NAME_BLEND,
	0,
	FORMAT_BLEND,
	0,
	OFFSET_BLEND,
	D3D11_INPUT_PER_VERTEX_DATA,
	0
};

static D3D11_INPUT_ELEMENT_DESC DEFAULT_SPEC_MOD_VERT_DATA =
{
	NAME_SPECMOD,
	0,
	FORMAT_SPECMOD,
	0,
	OFFSET_SPECMOD,
	D3D11_INPUT_PER_VERTEX_DATA,
	0
};


// ----------------------------------------------------------
//  Layouts -------------------------------------------------
// ----------------------------------------------------------
static D3D11_INPUT_ELEMENT_DESC IA_BLENDED_TILE[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_UV_VERT_DATA,
	DEFAULT_NORMAL_VERT_DATA,
	DEFAULT_BLEND_WEIGHT_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_COLOR_LIGHT[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_NORMAL_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_COLOR_SELECTED[] =
{
	DEFAULT_POS_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_SIMPLE_COLOR[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_COLOR_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_SIMPLE_TEXTURE[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_UV_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_TEXTURE_LIGHT[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_UV_VERT_DATA,
	DEFAULT_NORMAL_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_SPECTRUM_LIGHT[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_SPEC_MOD_VERT_DATA,
	DEFAULT_NORMAL_VERT_DATA
};

static D3D11_INPUT_ELEMENT_DESC IA_MAGNITUDE_LIGHT[] =
{
	DEFAULT_POS_VERT_DATA,
	DEFAULT_SPEC_MOD_VERT_DATA,
	DEFAULT_NORMAL_VERT_DATA
};


ShaderCompileHelper GetInputLayout(ShaderName name);