
float Uv;
float Power;

texture TexSrc;
texture TexDst;

sampler Sampler0 = sampler_state
{
    Texture = <TexSrc>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler Sampler1 = sampler_state
{
    Texture = <TexDst>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

struct VS_OUTPUT
{
    float2 Tex : TEXCOORD0;
};

float4 PS(VS_OUTPUT In) : COLOR
{
    float4 dst = tex2D(Sampler1, In.Tex + Uv);
    float2 index;
    index.x = In.Tex.x + dst.r * Power * 0.1;
    index.y = In.Tex.y;
    float4 src = tex2D(Sampler0, index);
    return src;
}

technique main
{
    Pass P0
    {
        PixelShader = compile ps_2_0 PS();
    }
}