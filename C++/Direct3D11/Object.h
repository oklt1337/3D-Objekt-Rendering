#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include "Material.h"
#include "Mesh.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class object
{
public:
    INT init(ID3D11Device* p_d_3d_dev, material::textures texture, mesh::forms form, XMFLOAT3 position, XMFLOAT3 scale, material::shader shader);
    void render(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT4X4* p_view_matrix, const XMFLOAT4X4* p_projection_matrix, const XMFLOAT3* p_camera_pos);
    void update(FLOAT dt);
    void de_init();

private:
    material material_ = {};
    mesh mesh_ = {};
};
