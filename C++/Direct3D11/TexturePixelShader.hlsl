Texture2D main_texture_2d;
sampler main_sampler;

struct pixel_input
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(pixel_input input) : SV_TARGET
{
	return main_texture_2d.Sample(main_sampler, input.uv);
}