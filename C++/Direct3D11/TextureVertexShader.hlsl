cbuffer matrix_buffer
{
	float4x4 wvp_matrix;
}

struct vertex_input
{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
};

struct vertex_output
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

vertex_output main(vertex_input input)
{
	vertex_output output;
	output.pos = mul(float4(input.pos, 1.0f), wvp_matrix);
	output.uv = input.uv;
	
	return output;
}