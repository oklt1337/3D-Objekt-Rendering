#include "Mesh.h"
#include <DirectXMath.h>
#include <xutility>

#include "mesh_data_utils.h"
#include "Utils.h"
#include "Vertex.h"

using namespace DirectX;

INT mesh::init(ID3D11Device* p_d_3d_dev, const forms form, const XMFLOAT3 position, const XMFLOAT3 scale)
{
    position_ = position;
    scale_ = scale;

    if (form == cube)
    {
        mesh_data_ = mesh_data_utils::create_cube(scale_);
    }
    else if (form == sphere)
    {
        mesh_data_ = mesh_data_utils::create_sphere(scale_.x * 0.5f ,100,100);
    }
    else if (form == cylinder)
    {
        mesh_data_ = mesh_data_utils::create_cylinder(scale_.x * 0.5f, scale_.z / 100.0f, scale_.y, 100, 100);
    }

    INT error = init_vertex_buffer(p_d_3d_dev);
    CHECK_ERROR(error);

    error = init_index_buffer(p_d_3d_dev);
    CHECK_ERROR(error);

    // init world transformation matrix
    XMStoreFloat4x4(&world_matrix_, XMMatrixIdentity());

    return 0;
}

void mesh::update(const FLOAT dt)
{
    static FLOAT rot = 0.0f;
    rot += XM_PI / 5.0f * dt;

    const XMMATRIX translation = XMMatrixTranslation(position_.x, position_.y, position_.z);
    const XMMATRIX rotation = XMMatrixRotationRollPitchYaw(0.0f, rot, 0.0f);
    const XMMATRIX local_scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);

    XMStoreFloat4x4(&world_matrix_, local_scale * rotation * translation);

}

void mesh::render(ID3D11DeviceContext* p_d_3d_dev_context) const
{
    // set vertex buffer source
    static UINT offset = 0;
    p_d_3d_dev_context->IASetVertexBuffers(0, 1, &p_vertex_buffer_, &vertex_stride_, &offset);
    p_d_3d_dev_context->IASetIndexBuffer(p_index_buffer_, DXGI_FORMAT_R16_UINT, offset);
    p_d_3d_dev_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //draw with index buffer
    p_d_3d_dev_context->DrawIndexed(index_count_, 0, 0);
}

void mesh::de_init()
{
    safe_release<ID3D11Buffer>(p_vertex_buffer_);
    safe_release<ID3D11Buffer>(p_index_buffer_);
}

INT mesh::init_vertex_buffer(ID3D11Device* p_d_3d_dev)
{
    D3D11_SUBRESOURCE_DATA initial_data = {};
    vertex_stride_ = sizeof(vertex);
    vertex_count_ = std::size(mesh_data_.vertices);
    initial_data.pSysMem = &mesh_data_.vertices[0];

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = vertex_count_ * vertex_stride_;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.Usage = D3D11_USAGE_IMMUTABLE;

    const HRESULT hr = p_d_3d_dev->CreateBuffer(&desc, &initial_data, &p_vertex_buffer_);
    CHECK_FAILED(hr, 30);

    return 0;
}

INT mesh::init_index_buffer(ID3D11Device* p_d_3d_dev)
{
    D3D11_SUBRESOURCE_DATA initial_data = {};
    index_count_ = std::size(mesh_data_.indices);
    initial_data.pSysMem = &mesh_data_.indices[0];

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = index_count_ * sizeof(USHORT);
    desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    desc.Usage = D3D11_USAGE_IMMUTABLE;

    const HRESULT hr = p_d_3d_dev->CreateBuffer(&desc, &initial_data, &p_index_buffer_);
    CHECK_FAILED(hr, 32);

    return 0;
}
