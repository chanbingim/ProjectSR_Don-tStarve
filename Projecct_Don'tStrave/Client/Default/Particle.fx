
float   vScale;
float4x4 WorldMat;
float4x4 ViewMat;
float4x4 ProjdMat;

texture Tex;

sampler Samp = sampler_state
{
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = POINT;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

struct VS_INPUT
{
    float4  Pos : POSITION;
    float2  Tex : TEXCOORD0;
    float   Size : PSIZE;
};

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;
    float4 LoaclPoint = float4(0, 0, 0, 1);
    
    float4 NormalRight = normalize(WorldMat[0]);
    float4 NormalUp = normalize(WorldMat[1]);
    float4 NormalLook = normalize(WorldMat[2]);
    
    NormalRight *= input.Size;
    NormalUp *= input.Size;
    NormalLook *= input.Size;
    
    float4x4 newWorldMat;
    newWorldMat[0] = NormalRight;
    newWorldMat[1] = NormalUp;
    newWorldMat[2] = NormalLook;
    newWorldMat[3] = input.Pos;
    
    float4 pos = mul(LoaclPoint, newWorldMat);
    pos = mul(pos, ViewMat);
    Out.Pos = mul(pos, ProjdMat);
    Out.Tex = input.Tex;
    return Out;
}

float4 PS(VS_OUTPUT In) : COLOR
{
    //return float4(1.f, 0.f, 1.f, 1.f);
    return tex2D(Samp, In.Tex);
}

technique main
{
    Pass P0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader = compile ps_2_0 PS();
    }
}