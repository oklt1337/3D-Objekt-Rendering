Texture2D main_texture_2d;
sampler main_sampler;

cbuffer light_data
{
	float3 light_direction;
	float light_intensity;
	float4 light_diffuse_color;
	float4 ambient_color;
}

struct pixel_input
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
};

float4 main(const pixel_input input) : SV_TARGET
{
	float4 texture_color = main_texture_2d.Sample(main_sampler, input.uv);

	const float3 normal = normalize(input.normal);
	const float3 light = normalize(-light_direction);

	const float diffuse = max(dot(normal, light), 0);
	const float4 diffuse_color = light_diffuse_color * diffuse * light_intensity;
	
	return texture_color * saturate(ambient_color + diffuse_color);
}
