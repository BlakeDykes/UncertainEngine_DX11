//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

#pragma pack_matrix( row_major )

cbuffer WVPBuffer : register( b0 )
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos   : POSITION;
    float4 Color : COLOR;
    float4 Norm  : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos   : SV_POSITION;
    float4 Color : COLOR;
    float4 Norm  : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS( VS_INPUT input)
{
    VS_OUTPUT output;
    output.Pos = mul( input.Pos, World);
    output.Pos = mul( output.Pos, View);
    output.Pos = mul( output.Pos, Projection);
    output.Color = input.Color;
    output.Norm = input.Norm;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS( VS_OUTPUT input ) : SV_Target
{
    return input.Color;
}
