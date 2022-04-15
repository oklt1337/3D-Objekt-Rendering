#include "helper_functions.hlsli"

cbuffer light_data
{
	float3 light_direction;
	float light_intensity;
	float4 light_diffuse_color;
	float4 ambient_color;
	float4 specular_color;
	float specular_power;
	float glossy;
	float4 self_illumination_color;
}

struct pixel_input
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bi_normal : BINORMAL;
	float2 uv : TEXCOORD0;
	float3 view_direction : TEXCOORD1;
};

float4 main(const pixel_input input) : SV_TARGET
{
	const float4 texture_color = main_texture_2d.Sample(main_sampler, input.uv);
	const float4 a_color = ambient_color;
	const float4 emission = self_illumination_color;

	const float3 normal = calculate_normal(input.tangent, input.bi_normal, input.normal, input.uv);
	const float3 light = normalize(-light_direction);
	const float3 view_dir = normalize(input.view_direction);

	// diffuse color
	const float diffuse = max(dot(normal, light), 0);
	const float4 diffuse_color = light_diffuse_color * diffuse * light_intensity;

	// specular color

	//Phong
	const float3 reflect_vector = 2 * diffuse * normal - light;
	const float specular = pow(max(dot(reflect_vector, view_dir), 0), specular_power) * glossy;

	//Blinn Phong
	//const float3 half_vec = normalize(light + view_dir);
	//const float specular = pow(max(dot(normal, half_vec), 0), specular_power) * glossy;

	const float4 s_color = specular_color * specular;

	return saturate(texture_color * saturate(a_color + diffuse_color) + s_color + emission);
}
