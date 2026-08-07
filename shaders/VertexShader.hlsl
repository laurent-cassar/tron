// Tourne une fois par sommet : transforme la position 3D en position ecran.
cbuffer TransformBuffer : register(b0)
{
    matrix WVP; // World * View * Projection (temporaire, voir Renderer.cpp)
};

struct VS_INPUT
{
    float3 pos   : POSITION;
    float3 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos   : SV_POSITION;
    float3 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(float4(input.pos, 1.0f), WVP);
    output.color = input.color;
    return output;
}
