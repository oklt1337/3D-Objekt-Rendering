#include "helper_functions.hlsli"

cbuffer light_data
{
	float3 light_direction;
	float light_intensity;
	float4 diffuse_color;
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
	float2 uv : TEXCOORD0;
};

float4 main(const pixel_input input) : SV_TARGET
{
	float3 texture_color = main_texture_2d.Sample(main_sampler, input.uv);
	const float3 normal = normalize(input.normal);

	//light
	const float3 light_dir = normalize(-light_direction);
	const float3 light_col = diffuse_color.rgb;
	
	// diffuse light
	const float light_falloff = max(0, dot(light_dir, normal));
	const float3 direct_diffuse_light = light_col * light_falloff;

	// ambient light
	const float3 ambient_light = ambient_color.rgb;
	
	//composite
	const float3 diffuse_light = ambient_light + direct_diffuse_light;
	float3 final_surface_col = diffuse_light * texture_color.rgb;

	return float4(final_surface_col, 0);
}
