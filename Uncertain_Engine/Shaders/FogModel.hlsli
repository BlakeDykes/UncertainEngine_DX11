
float SimpleFogPercent(
    float start,
    float range,
    float4 msEyePos,
    float4 msPos
)
{
    if (range <= 0.0f)
    {
        return 0.0f;
    }
    
    float distToEye = length(msEyePos - msPos);
    return saturate((distToEye - start) / range);
}