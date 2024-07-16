#pragma pack_matrix( row_major )

struct PhongADS
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

struct DirectionalLight
{
    PhongADS Light;
    float4 Direction;
};

struct PointLight
{
    PhongADS Light;
    float4 Position;
    float4 Attenuation;
    //float Range;
};

struct SpotLight
{
    PhongADS Light;
    float4 Position;
    float4 Attenuation;
    float4 Direction;
    //float Range;
    //float SpotExp;
};

struct PhongMaterial
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

void PhongModel(
    PhongMaterial mat,
    PhongADS lightint,
    float3 L,
    float3 normal,
    float3 DirToEye,
    out float4 ambient,
    out float4 diffuse,
    out float4 spec
)
{
    ambient = mat.Ambient * float4(lightint.Ambient.xyz, 1.0);
    
    float diffuseFactor = dot(L, normal);
    
    if (diffuseFactor > 0.0f)
    {
        diffuse = diffuseFactor * mat.Diffuse * float4(lightint.Diffuse.xyz, 1.0);
        
        float3 r = reflect(-L, normal);
        float spec_val = max(dot(r, DirToEye.xyz), 0);
        float specFactor = pow(spec_val, mat.Specular.w);
        spec = specFactor * mat.Specular * lightint.Specular;
    }
    else
    {
        diffuse = float4(0, 0, 0, 0);
        spec    = float4(0, 0, 0, 0);
    }
}

void ComputeDirectionalLight(
    PhongMaterial mat,
    float4x4 worldInv,
    DirectionalLight dirLight,
    float4 normal,
    float4 dirToEye,
    out float4 ambient,
    out float4 diffuse,
    out float4 spec
)
{
    if (dirLight.Light.Specular.w < -50.0f)
        return;
    
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    spec =    float4(0, 0, 0, 0);
    
    float3 L = normalize(mul(-dirLight.Direction.xyz, (float3x3) worldInv));
    
    PhongModel(mat, dirLight.Light, L, normal.xyz, dirToEye.xyz, ambient, diffuse, spec);
}

void ComputePointLight(
    PhongMaterial mat,
    float4x4 worldInv,
	PointLight pLight,
	float4 posms,
	float4 normal,
	float4 dirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
    if (pLight.Light.Specular.w < -50.0f)
        return;
    
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    spec = float4(0, 0, 0, 0);
    
    float3 lightPosMS = mul(pLight.Position, worldInv).xyz;
    float3 L = lightPosMS - posms.xyz;
    
    float d = length(L);
    if (d > pLight.Light.Diffuse.w)
        return;
 
    L /= d;
    
    PhongModel(mat, pLight.Light, L, normal.xyz, dirToEye.xyz, ambient, diffuse, spec);
    
    float att = 1 / dot(pLight.Attenuation.xyz, float3(1, d, d * d));
    
    diffuse *= att;
    spec *= att;
}

void ComputeSpotLight(
	PhongMaterial mat,
    float4x4 worldInv,
	SpotLight spotLight,
	float4 posms,
	float4 normal,
	float4 dirToEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec
)
{
    if (spotLight.Light.Specular.w < -50.0f)
        return;
    
    ambient = float4(0, 0, 0, 0);
    diffuse = float4(0, 0, 0, 0);
    spec = float4(0, 0, 0, 0);

 
    float3 lightPos = mul(spotLight.Position, worldInv).xyz;
    float3 L = lightPos - posms.xyz;
    
    float d = length(L);
    if (d > spotLight.Direction.w)
        return;
    
    L /= d;
    
    PhongModel(mat, spotLight.Light, L, normal.xyz, dirToEye.xyz, ambient, diffuse, spec);
    
    float att = 1 / dot(spotLight.Attenuation.xyz, float3(1, d, d * d));
    
    diffuse *= att;
    spec *= att;
    
    float4 msLightDir = float4(spotLight.Direction.xyz, 0.0);
    float spotExp = spotLight.Light.Diffuse.w;
    
    msLightDir = normalize(mul(msLightDir, worldInv));
    float spot = pow(max(dot(-L, msLightDir.xyz), 0), spotExp);
    
    ambient *= spot;
    diffuse *= spot;
    spec *= spot;
}