#pragma pack_matrix( row_major )

#include "PhongModel.hlsli"
#include "FogModel.hlsli"

Texture2D rTex : register(t0);
Texture2D gTex : register(t1);
Texture2D bTex : register(t2);
Texture2D aTex : register(t3);

SamplerState aSampler : register(s0);

cbuffer CamData : register(b0)
{
    float4x4 View;
    float4x4 Projection;
    float4 EyePosWorld;
};

cbuffer InstanceData : register(b1)
{
    float4x4 World;
    float4x4 WorldInv;
    PhongMaterial Material;
};

cbuffer LightData : register(b2)
{
    DirectionalLight DirLight;
    PointLight PLight;
    SpotLight SLight;
};

cbuffer FogData : register(b3)
{
    float FogRange;
    float FogStart;
    float4 FogColor;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float4 Norm : NORMAL;
    float4 Blend : BLEND;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
    float2 Tex : TEXCOORD;
    float4 Norm : NORMAL;
    float4 Blend : BLEND;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.PosMS = input.Pos;
    output.Norm = input.Norm;
    output.Tex = input.Tex;
    output.Blend = input.Blend;
    
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
    if (input.Blend.r + input.Blend.g >= 1.99f) discard;
    
    float4 msEyePos = mul(EyePosWorld, WorldInv);
    float4 msDirToEye = normalize(msEyePos - input.PosMS);
    
    float4 ambient = float4(0, 0, 0, 0);
    float4 diffuse = float4(0, 0, 0, 0);
    float4 spec = float4(0, 0, 0, 0);

    float4 A, D, S;
    
    ComputeDirectionalLight(Material, WorldInv, DirLight, normalize(input.Norm), msDirToEye, A, D, S);

    ambient += A;
    diffuse += D;
    spec += S;

    ComputePointLight(Material, WorldInv, PLight, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

    ambient += A;
    diffuse += D;
    spec += S;

    ComputeSpotLight(Material, WorldInv, SLight, input.PosMS, normalize(input.Norm), msDirToEye, A, D, S);

    ambient += A;
    diffuse += D;
    spec += S;
    
        
    float fogPercent = SimpleFogPercent(FogStart, FogRange, msEyePos, input.PosMS);
    
    float4 litColor = (ambient + diffuse + spec) * (1.0f - fogPercent) + (FogColor * fogPercent);
    
    float4 texColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    texColor += rTex.Sample(aSampler, input.Tex) * input.Blend.r;
    texColor += gTex.Sample(aSampler, input.Tex) * input.Blend.g;
    texColor += bTex.Sample(aSampler, input.Tex) * input.Blend.b;
    texColor += aTex.Sample(aSampler, input.Tex) * input.Blend.a;
    
    return  texColor * litColor;
}
