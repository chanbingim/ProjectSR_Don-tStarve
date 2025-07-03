
float3 vScale;

float4x4 WorldMat;
float4x4 ViewMat;
float4x4 ProjdMat;

texture Tex;

sampler Samp = sampler_state
{
    Texture = <Tex>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;
    
    float4 NormalRight = normalize(WorldMat[0]);
    float4 NormalUp = normalize(WorldMat[1]);
    float4 NormalLook = normalize(WorldMat[2]);
    
    NormalRight *= vScale.x;
    NormalUp *= vScale.y;
    NormalLook *= vScale.z;
    
    float4x4 newWorldMat = float4x4(NormalRight, NormalUp, NormalLook, WorldMat[3]);
    float4 pos = mul(input.Pos, newWorldMat);
    pos = mul(pos, ViewMat);
    Out.Pos = mul(pos, ProjdMat);
    
    Out.Tex = input.Tex;

    return Out;
}

//float4 PS(VS_OUTPUT In) : COLOR
//{
//    return tex2D(Samp, In.Tex);
//}

technique main
{
    Pass P0
    {
        VertexShader = compile vs_2_0 VS();
        //PixelShader = compile ps_2_0 PS();

    }
}