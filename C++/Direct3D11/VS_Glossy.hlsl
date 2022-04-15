cbuffer matrix_buffer
{
float4x4 wvp_matrix;
float4x4 world_matrix;
}

cbuffer frame_buffer
{
float3 camera_pos;
float buffer;
}

struct vertex_input
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct vertex_output
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD0;
    float3 view_direction : TEXCOORD1;
};

vertex_output main(vertex_input input)
{
    vertex_output output;
    output.pos = mul(float4(input.pos, 1.0), wvp_matrix);
    output.normal = normalize(mul(input.normal, (float3x3)world_matrix));
    output.uv = input.uv;
    output.view_direction = normalize(camera_pos - mul(world_matrix, float4(input.pos, 1.0)).xyz);

    return output;
}
