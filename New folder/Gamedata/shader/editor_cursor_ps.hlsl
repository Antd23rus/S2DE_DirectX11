struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 worldPos		: POSITION; 
};

float4 main(PSINPUT input) : SV_TARGET
{
    return input.color;
}