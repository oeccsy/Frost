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

[maxvertexcount(4)]
void GS(point VS_OUTPUT input[1], inout TriangleStream<VS_OUTPUT> stream)
{
    float size = 0.1;
    
    float4 center = input[0].position;
    float4 color = input[0].color;
    
    float x[4] = { size, -size, -size, size };
    float y[4] = { size, size, -size, -size };
    VS_OUTPUT vertices[4];
    
    for (int i = 0; i < 4; i++)
    {
        vertices[i].position = center + float4(x[i], y[i], 0, 0);
        vertices[i].color = color;
    }
    
    // triangle strip은 이전 두 개 vertex를 활용한 clockwise
    stream.Append(vertices[3]); // 우하단
    stream.Append(vertices[2]); // 좌하단
    stream.Append(vertices[0]); // 우상단
    stream.Append(vertices[1]); // 좌상단
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.color;
}