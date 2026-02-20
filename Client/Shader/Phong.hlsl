cbuffer TransformData : register(b0)
{
    row_major matrix world_matrix;
    row_major matrix view_matrix;
    row_major matrix proj_matrix;
}

struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(input.position, world_matrix);
    output.position = mul(output.position, view_matrix);
    output.position = mul(output.position, proj_matrix);
    output.normal = mul(input.normal, world_matrix);
    output.normal = mul(input.normal, view_matrix);
    output.normal = mul(input.normal, proj_matrix);
    output.uv = input.uv;
    output.color = input.color;
    
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    // ambient
    float3 ambient = float3(0.5f, 0.5f, 0.5f);
    
    // diffuse
    float3 normal_vec = normalize(input.normal);
    float3 light_col = float3(0.9f, 0.9f, 0.9f); // constant
    float3 light_vec = float3(1.0f, 1.0f, 1.0f); // constant
    
    float diffuse_str = max(dot(light_vec, normal_vec), 0.0f);
    float3 diffuse = diffuse_str * light_col;
    
    // specular
    float3 camera_pos = float3(0.0f, 0.0f, 10.0f); // constant
    float3 view_vec = normalize(camera_pos);
    float3 reflect_vec = normalize(reflect(-light_vec, normal_vec));
    float specular_str = max(dot(view_vec, reflect_vec), 0.0f);
    specular_str = pow(specular_str, 256.0f);
    float3 specular = specular_str * light_col;
    
    // lighting = ambient + diffuse + specular
    float3 lighting = float3(0.0f, 0.0f, 0.0f);
    lighting += ambient * 0.1f;
    lighting += diffuse * 0.5f;
    lighting += specular * 0.5f;
    
    // color = model color * lighting
    float3 model_color = input.color;
    float4 color = float4(model_color * lighting, 1.0f);
    
    return color;
}