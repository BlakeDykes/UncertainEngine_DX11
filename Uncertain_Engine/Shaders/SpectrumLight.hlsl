#pragma pack_matrix( row_major )

#include "PhongModel.hlsli"
#include "SpectralModel.hlsli"

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

cbuffer AudioData : register(b4)
{
    SpectrumData ASpectrum;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float3 Pos              : POSITION;
    int    SpectrumModifier : SPECMOD;
    uint   SpectrumIndex    : SPECINDEX;
    float3 Norm             : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 PosMS : POSITION;
    float4 Norm : NORMAL;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    float heightMod = 0.0f;
    if (input.SpectrumModifier > 0)
    {
        heightMod = (ASpectrum.Data[input.SpectrumIndex / 4][input.SpectrumIndex % 4] / 3.0f);
    }
    
    input.Pos.y += heightMod;
    
    VS_OUTPUT output;
    output.PosMS = float4(input.Pos, 1.0f);
    output.Norm = float4(input.Norm, 1.0f);
    
    output.Pos = mul(float4(input.Pos, 1.0f), World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
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
    
    return ambient + diffuse + spec;
}
