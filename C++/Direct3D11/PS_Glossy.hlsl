Texture2D main_texture_2d;
sampler main_sampler;

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
    float3 view_direction : TEXCOORD1;
};

float4 main(const pixel_input input) : SV_TARGET
{
    float3 texture_color = main_texture_2d.Sample(main_sampler, input.uv);
    const float3 normal = normalize(input.normal);
    const float3 view_dir = normalize(input.view_direction);
    
    //light
    const float3 light_dir = normalize(-light_direction);
    const float3 light_col = diffuse_color.rgb;
	
    // diffuse light
    float light_falloff = max(0, dot(light_dir, normal));
    light_falloff = step(0.6, light_falloff);
    const float3 direct_diffuse_light = light_col * light_falloff;

    // ambient light
    const float3 ambient_light = ambient_color.rgb;

    // specular light
    const float3 view_reflect = reflect(-view_dir, normal);
    float specular_falloff = max(0, dot(view_reflect, light_dir));
    specular_falloff = pow(specular_falloff, specular_power) * glossy;
    specular_falloff = step(0.1, specular_falloff);
    const float3 direct_specular = specular_falloff * light_col;
	
    //composite
    const float3 diffuse_light = ambient_light + direct_diffuse_light;
    float3 final_surface_col = saturate(diffuse_light * texture_color.rgb + direct_specular);

    return float4(final_surface_col, 0);
}
