
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
    
      // 전경의 알파도 반영하고 싶다면
    float a = dst.a * Alpha;

    // 자연스러운 알파 블렌딩
    float3 rgb = lerp(src.rgb, dst.rgb, a);
    return src + float4(rgb, 1.0 - Alpha); // 알파는 보통 무시 (렌더타겟용이면)

}

technique main
{
    Pass P0
    {
        PixelShader = compile ps_2_0 PS();
    }
}