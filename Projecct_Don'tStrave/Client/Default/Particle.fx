
float Alpha;
float PlayerHP;
float Uv;
float Power;
float End;
float Ending;

texture TexSrc;
texture TexDst;
texture TexArg;
texture TexEnd;

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

sampler Sampler2 = sampler_state
{
    Texture = <TexArg>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
};

sampler Sampler3 = sampler_state
{
    Texture = <TexEnd>;
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
    float4 dst = tex2D(Sampler1, In.Tex);
    float4 arg = tex2D(Sampler2, In.Tex + Uv);
    float4 end = tex2D(Sampler3, In.Tex) * Ending;
    
    float2 index;
    index.x = In.Tex.x + arg.r * Power * 0.2;
    index.y = In.Tex.y;
    float4 src = tex2D(Sampler0, index);
    
    float gray = dot(src.rgb, float3(0.299, 0.587, 0.114));
    
    if (0 >= PlayerHP)
        src = lerp(src, float4(gray, gray, gray, 1.f), 1.f);
    
    
    return lerp(src, dst, Alpha) * float4(End, End, End, 1) + end;
}

technique main
{
    Pass P0
    {
        PixelShader = compile ps_2_0 PS();
    }
}