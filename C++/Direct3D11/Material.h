#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class material
{
public:
    enum textures { wall, gray, floor, dragon, solid };
    enum shader { light, matt, rough, glossy };

    INT init(ID3D11Device* p_d_3d_dev, textures texture, shader shader);
    void render(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT4X4* p_world_matrix, const XMFLOAT4X4* p_view_matrix,
                const XMFLOAT4X4* p_projection_matrix, const XMFLOAT3* p_camera_pos) const;
    void de_init();
    

private:
    INT create_vertex_shader(ID3D11Device* p_d_3d_dev, shader shader);
    INT create_pixel_shader(ID3D11Device* p_d_3d_dev, shader shader);
    INT create_input_layout(ID3D11Device* p_d_3d_dev, ID3DBlob* p_vertex_shader_data);
    INT create_matrix_buffer(ID3D11Device* p_d_3d_dev);
    INT create_frame_buffer(ID3D11Device* p_d_3d_dev);
    void set_matrices(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT4X4* p_world_matrix, const XMFLOAT4X4* p_view_matrix,
                      const XMFLOAT4X4* p_projection_matrix) const;
    void set_frame_buffer(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT3* p_camera_pos) const;
    INT create_texture_sampler(ID3D11Device* p_d_3d_dev, textures texture);
    INT create_normal_map_sampler(ID3D11Device* p_d_3d_dev, textures texture);

    ID3D11VertexShader* p_vertex_shader_ = nullptr;
    ID3D11PixelShader* p_pixel_shader_ = nullptr;
    ID3D11InputLayout* p_input_layout_ = nullptr;

    ID3D11Buffer* p_matrix_buffer_ = nullptr;

    struct matrix_buffer
    {
        XMFLOAT4X4 world_view_projection_matrix;
        XMFLOAT4X4 world_matrix;
    };

    ID3D11Buffer* p_frame_buffer_ = nullptr;
    struct frame_buffer
    {
        XMFLOAT3 camera_pos;
        FLOAT buffer;
    };

    ID3D11ShaderResourceView* p_shader_resource_view_ = nullptr;
    ID3D11ShaderResourceView* p_shader_resource_view2_ = nullptr;
    ID3D11SamplerState* p_sampler_state_ = nullptr;
};