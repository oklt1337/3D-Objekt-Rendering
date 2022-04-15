#include "mesh_data_utils.h"

mesh_data mesh_data_utils::create_sphere(const float radius, const int slice_count, const int stack_count)
{
    mesh_data mesh_data = {};
    mesh_data.vertices.push_back(vertex(0.0f, radius, 0.0f, 0.0f, 1.0f, 0.0f,1.0f,0.0f,0.0f, 0.0f, 0.0f));
    const float phi_step = XM_PI / stack_count;
    const float theta_step = 2.0f * XM_PI / slice_count;

    for (int i = 1; i <= stack_count - 1; i++)
    {
        const float phi = i * phi_step;
        for (int j = 0; j <= slice_count; j++)
        {
            const float theta = j * theta_step;
            const XMFLOAT3 p =
            {
                (radius * XMScalarSin(phi) * XMScalarCos(theta)),
                (radius * XMScalarCos(phi)),
                (radius * XMScalarSin(phi) * XMScalarSin(theta))
            };

            XMFLOAT3 t =
            {
                (-radius * XMScalarSin(phi) * XMScalarSin(theta)),
                (0),
                (radius * XMScalarSin(phi) * XMScalarCos(theta))
            };
            XMVector3Normalize(XMLoadFloat3(&t));
            XMFLOAT3 n = p;
            XMVector3Normalize(XMLoadFloat3(&n));

            const auto uv = XMFLOAT2
            {
                theta / (XM_PI * 2),
                phi / XM_PI
            };
            mesh_data.vertices.push_back(vertex(p.x, p.y, p.z, n.x, n.y, n.z,t.x, t.y, t.z ,uv.x, uv.y));
        }
    }
    mesh_data.vertices.push_back(vertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));

    for (int i = 1; i <= slice_count; i++)
    {
        mesh_data.indices.push_back(0);
        mesh_data.indices.push_back(i + 1);
        mesh_data.indices.push_back(i);
    }

    auto base_index = 1;
    const auto ring_vertex_count = slice_count + 1;
    for (int i = 0; i < stack_count - 2; i++)
    {
        for (int j = 0; j < slice_count; j++)
        {
            mesh_data.indices.push_back(base_index + i * ring_vertex_count + j);
            mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
            mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);

            mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
            mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
            mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
        }
    }
    const auto south_pole_index = mesh_data.indices.size() - 1;
    base_index = south_pole_index - ring_vertex_count;
    for (int i = 0; i < slice_count; i++)
    {
        mesh_data.indices.push_back(south_pole_index);
        mesh_data.indices.push_back(base_index + i);
        mesh_data.indices.push_back(base_index + i + 1);
    }

    //callculate tangent and binormal
    
    return mesh_data;
}

mesh_data mesh_data_utils::create_cube(const XMFLOAT3 scale)
{
    mesh_data mesh_data = {};

    const auto w2 = 0.5f * scale.x;
    const auto h2 = 0.5f * scale.y;
    const auto d2 = 0.5f * scale.z;

    // front
    mesh_data.vertices.push_back(vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
    // back
    mesh_data.vertices.push_back(vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
    // top
    mesh_data.vertices.push_back(vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
    // bottom
    mesh_data.vertices.push_back(vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
    // left
    mesh_data.vertices.push_back(vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));
    // right
    mesh_data.vertices.push_back(vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
    mesh_data.vertices.push_back(vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f));
    mesh_data.vertices.push_back(vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f));

    USHORT indices[] =
    {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    mesh_data.indices.insert(mesh_data.indices.begin(), std::begin(indices), std::end(indices));

    return mesh_data;
}

mesh_data mesh_data_utils::create_cylinder(const float bottom_radius, const float top_radius, const float height,
                                           const int slice_count,
                                           const int stack_count)
{
    mesh_data mesh_data = {};

    const auto stack_height = height / stack_count;
    const auto radius_step = (top_radius - bottom_radius) / stack_count;
    const auto ring_count = stack_count + 1;

    for (int i = 0; i < ring_count; i++)
    {
        const auto y = -0.5f * height + i * stack_height;
        const auto r = bottom_radius + i * radius_step;
        const auto d_theta = 2.0f * XM_PI / slice_count;
        for (int j = 0; j <= slice_count; j++)
        {
            const auto c = XMScalarCos(j * d_theta);
            const auto s = XMScalarSin(j * d_theta);

            const auto v = XMFLOAT3(r * c, y, r * s);
            const auto uv = XMFLOAT2(static_cast<float>(j) / slice_count, 1.0f - static_cast<float>(i) / stack_count);
            auto t = XMFLOAT3(-s, 0.0f, c);

            const auto dr = bottom_radius - top_radius;
            auto bi_tangent = XMFLOAT3(dr * c, -height, dr * s);

            const auto n = XMVector3Cross(XMLoadFloat3(&t), XMLoadFloat3(&bi_tangent));
            XMVector3Normalize(n);
            XMFLOAT3 n_float3;
            XMStoreFloat3(&n_float3, n);
            mesh_data.vertices.push_back(vertex(v.x, v.y, v.z, n_float3.x, n_float3.y, n_float3.z, t.x, t.y, t.z, uv.x,
                                                uv.y));
        }
    }
    const auto ring_vertex_count = slice_count + 1;
    for (int i = 0; i < stack_count; i++)
    {
        for (int j = 0; j < slice_count; j++)
        {
            mesh_data.indices.push_back(i * ring_vertex_count + j);
            mesh_data.indices.push_back((i + 1) * ring_vertex_count + j);
            mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);

            mesh_data.indices.push_back(i * ring_vertex_count + j);
            mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
            mesh_data.indices.push_back(i * ring_vertex_count + j + 1);
        }
    }
    build_cylinder_top_cap(top_radius, height, slice_count, &mesh_data);
    build_cylinder_bottom_cap(bottom_radius, height, slice_count, &mesh_data);

    return mesh_data;
}

void mesh_data_utils::build_cylinder_top_cap(const float top_radius, const float height, const int slice_count,
                                             mesh_data* mesh_data)
{
    const auto base_index = mesh_data->vertices.size();

    const auto y = 0.5f * height;
    const auto d_theta = 2.0f * XM_PI / slice_count;

    for (int i = 0; i <= slice_count; i++)
    {
        const auto x = top_radius * XMScalarCos(i * d_theta);
        const auto z = top_radius * XMScalarSin(i * d_theta);

        const auto u = x / height + 0.5f;
        const auto v = z / height + 0.5f;
        mesh_data->vertices.push_back(vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
    }

    mesh_data->vertices.push_back(vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

    const auto center_index = base_index - 1;
    for (int i = 0; i < slice_count; i++)
    {
        mesh_data->indices.push_back(center_index);
        mesh_data->indices.push_back(base_index + i + 1);
        mesh_data->indices.push_back(base_index + i);
    }
}

void mesh_data_utils::build_cylinder_bottom_cap(float bottom_radius, float height, int slice_count,
                                                mesh_data* mesh_data)
{
    const auto base_index = mesh_data->vertices.size();

    const auto y = -0.5f * height;
    const auto d_theta = 2.0f * XM_PI / slice_count;

    for (int i = 0; i <= slice_count; i++)
    {
        const auto x = bottom_radius * XMScalarCos(i * d_theta);
        const auto z = bottom_radius * XMScalarSin(i * d_theta);

        const auto u = x / height + 0.5f;
        const auto v = z / height + 0.5f;

        mesh_data->vertices.push_back(vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
    }

    mesh_data->vertices.push_back(vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

    const auto center_index = base_index - 1;
    for (int i = 0; i < slice_count; i++)
    {
        mesh_data->indices.push_back(center_index);
        mesh_data->indices.push_back(base_index + i);
        mesh_data->indices.push_back(base_index + i + 1);
    }
}
