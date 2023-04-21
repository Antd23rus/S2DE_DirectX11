Texture2D _texture;
SamplerState sampleType;

struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 worldPos		: POSITION; 
};

float4 main(PSINPUT input) : SV_TARGET
{
    float4 mask = _texture.Sample(sampleType, input.uv).r * float4(1, 0, 0, 0);
    float4 orig = _texture.Sample(sampleType, input.uv).r * input.color;
    
    return float4(orig.rgb, orig.a * mask.r);
}