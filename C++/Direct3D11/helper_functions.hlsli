Texture2D main_texture_2d;
Texture2D normal_map;
sampler main_sampler;

float3 calculate_normal(float3 tangent, float3 bi_normal, float3 normal, const float2 uv)
{
    tangent = normalize(tangent);
    bi_normal = normalize(bi_normal);
    normal = normalize(normal);

    float4 normal_map_color = normal_map.Sample(main_sampler, uv);
    normal_map_color = normal_map_color * 2 - 1;

    return normalize(tangent * normal_map_color.r + bi_normal * normal_map_color.g + normal * normal_map_color.b);
}