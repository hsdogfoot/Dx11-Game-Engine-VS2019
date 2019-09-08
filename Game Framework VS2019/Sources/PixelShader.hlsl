cbuffer mycBuffer : register(b0)
{
    float red;
    float green;
    float blue;
};

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 pixelColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    pixelColor += float4(red, green, blue, 0.0f);
    return pixelColor;
}