
float4x4 CameraWorldMat;
texture Tex;

sampler Samp = sampler_state
{
    Texture = <Tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;

    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT Out = (VS_OUTPUT)0;

    float4x4 WorldMat = CameraWorldMat;
    WorldMat[3] = input.Pos;
    
    Out.Pos = mul(input.Pos, CameraWorldMat);
    Out.Tex = input.Tex;

    return Out;
}

float4 PS(VS_OUTPUT In) : COLOR
{
    return tex2D(Samp, In.Tex);
}

technique BillboardShader
{
    pass P0
    {
        VertexShader = compile vs_1_1 VS();
        PixelShader = compile vs_1_1 PS();
    }
}