cbuffer offsetBuffer : register(b0)
{
    float xDetail;
    float yDetail;
    float xCount;
    float yCount;
};

cbuffer mycBuffer : register(b1)
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
    float2 texCoord = input.inTexCoord;
    texCoord.x = texCoord.x * xDetail + xCount * xDetail;
    texCoord.y = texCoord.y * yDetail + yCount * yDetail;

    float4 pixelColor = objTexture.Sample(objSamplerState, texCoord);
    if (pixelColor.a < 1.0f)
        discard;
    pixelColor += float4(red, green, blue, 0.0f);
    return pixelColor;
}