#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "mesh_data.h"

using namespace DirectX;

class mesh
{
public:
    enum forms { cube, cylinder, sphere };
    INT init(ID3D11Device* p_d_3d_dev, forms form, XMFLOAT3 position, XMFLOAT3 scale);
    void update(FLOAT dt);
    void render(ID3D11DeviceContext* p_d_3d_dev_context) const;
    void de_init();

    XMFLOAT4X4* get_world_matrix() { return &world_matrix_; };

private:
    INT init_vertex_buffer(ID3D11Device* p_d_3d_dev);
    INT init_index_buffer(ID3D11Device* p_d_3d_dev);

    ID3D11Buffer* p_vertex_buffer_ = nullptr;
    ID3D11Buffer* p_index_buffer_ = nullptr;

    UINT vertex_count_ = 0;
    UINT vertex_stride_ = 0;
    UINT index_count_ = 0;

    XMFLOAT4X4 world_matrix_ = {};
    XMFLOAT3 position_ = {};
    XMFLOAT3 scale_ = {};

    mesh_data mesh_data_ = {};
};
