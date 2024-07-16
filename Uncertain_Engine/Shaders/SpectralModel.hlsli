#pragma pack_matrix( row_major )

#define SampleCount_Spectrum  24
#define SampleCount_Magnitude 512


struct SpectrumData
{
    float4 Data[SampleCount_Spectrum / 4];
};

struct MagData
{
    float4 Data[SampleCount_Magnitude / 4];
};
