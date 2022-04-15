#include "Object.h"
#include "Utils.h"

using namespace DirectX;

INT object::init(ID3D11Device* p_d_3d_dev, const material::textures texture, const mesh::forms form,
                 const XMFLOAT3 position, const XMFLOAT3 scale, const material::shader shader)
{
    INT error = mesh_.init(p_d_3d_dev, form, position, scale);
    CHECK_ERROR(error);

    error = material_.init(p_d_3d_dev, texture, shader);
    CHECK_ERROR(error);

    return 0;
}

void object::render(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT4X4* p_view_matrix,
                    const XMFLOAT4X4* p_projection_matrix, const XMFLOAT3* p_camera_pos)
{
    material_.render(p_d_3d_dev_context, mesh_.get_world_matrix(), p_view_matrix, p_projection_matrix, p_camera_pos);
    mesh_.render(p_d_3d_dev_context);
}

void object::update(const FLOAT dt)
{
    mesh_.update(dt);
}

void object::de_init()
{
    mesh_.de_init();
    material_.de_init();
}
