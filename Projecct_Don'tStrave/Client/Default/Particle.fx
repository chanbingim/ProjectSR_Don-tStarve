
float Alpha;

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
    float4 src = tex2D(Sampler0, In.Tex);
    float4 dst = tex2D(Sampler1, In.Tex);
    
    return (src * (1.f - Alpha)) + (dst * (Alpha));

}

technique main
{
    Pass P0
    {
        PixelShader = compile ps_2_0 PS();
    }
}