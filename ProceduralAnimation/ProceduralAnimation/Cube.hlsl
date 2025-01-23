cbuffer TransformData : register(b0)
{
    row_major matrix worldMatrix;
    row_major matrix viewMatrix;
    row_major matrix projMatrix;
}

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projMatrix);
    output.uv = input.uv;
    output.color = input.color;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}