cbuffer matrix_buffer
{
	float4x4 wvp_matrix;
	float4x4 world_matrix;
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
	float2 uv : TEXCOORD;
};

vertex_output main(vertex_input input)
{
	vertex_output output;
	output.pos = mul(float4(input.pos, 1.0f), wvp_matrix);
	output.normal = normalize(mul(input.normal, (float3x3) world_matrix));
	output.uv = input.uv;
	
	return output;
}