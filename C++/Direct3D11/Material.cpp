#include "Material.h"
#include "WICTextureLoader.h"
#include "Utils.h"
#include <d3dcompiler.h>

#include "Camera.h"

using namespace DirectX;

INT material::init(ID3D11Device* p_d_3d_dev, const textures texture, const shader shader)
{
    INT error = create_vertex_shader(p_d_3d_dev, shader);
    CHECK_ERROR(error);

    error = create_pixel_shader(p_d_3d_dev, shader);
    CHECK_ERROR(error);

    error = create_matrix_buffer(p_d_3d_dev);
    CHECK_ERROR(error);

    error = create_frame_buffer(p_d_3d_dev);
    CHECK_ERROR(error);

    error = create_texture_sampler(p_d_3d_dev, texture);
    CHECK_ERROR(error);

    error = create_normal_map_sampler(p_d_3d_dev, texture);
    CHECK_ERROR(error);

    return 0;
}

void material::render(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT4X4* p_world_matrix,
                      const XMFLOAT4X4* p_view_matrix,
                      const XMFLOAT4X4* p_projection_matrix, const XMFLOAT3* p_camera_pos) const
{
    // set texture and sampler state
    p_d_3d_dev_context->PSSetShaderResources(0, 1, &p_shader_resource_view_);
    p_d_3d_dev_context->PSSetSamplers(0, 1, &p_sampler_state_);

    p_d_3d_dev_context->PSSetShaderResources(1, 1, &p_shader_resource_view2_);

    // set shader pipeline
    p_d_3d_dev_context->VSSetShader(p_vertex_shader_, nullptr, 0);
    p_d_3d_dev_context->PSSetShader(p_pixel_shader_, nullptr, 0);
    p_d_3d_dev_context->IASetInputLayout(p_input_layout_);

    set_matrices(p_d_3d_dev_context, p_world_matrix, p_view_matrix, p_projection_matrix);
    set_frame_buffer(p_d_3d_dev_context, p_camera_pos);
}

void material::de_init()
{
    safe_release<ID3D11VertexShader>(p_vertex_shader_);
    safe_release<ID3D11PixelShader>(p_pixel_shader_);
    safe_release<ID3D11InputLayout>(p_input_layout_);
    safe_release<ID3D11Buffer>(p_matrix_buffer_);
    safe_release<ID3D11Buffer>(p_frame_buffer_);
    safe_release<ID3D11ShaderResourceView>(p_shader_resource_view_);
    safe_release<ID3D11ShaderResourceView>(p_shader_resource_view2_);
    safe_release<ID3D11SamplerState>(p_sampler_state_);
}

INT material::create_vertex_shader(ID3D11Device* p_d_3d_dev, const shader shader)
{
    ID3DBlob* p_compiled_shader_code = nullptr;
    HRESULT hr = 0;
    // 2. load from already compiled shader
    if (shader == light)
    {
        hr = D3DReadFileToBlob(TEXT("LightVertexShader.cso"), &p_compiled_shader_code);
    }
    else if (shader == matt)
    {
        hr = D3DReadFileToBlob(TEXT("VS_Matt.cso"), &p_compiled_shader_code);
    }
    else if (shader == glossy)
    {
        hr = D3DReadFileToBlob(TEXT("VS_Glossy.cso"), &p_compiled_shader_code);
    }
    else if (shader == rough)
    {
        hr = D3DReadFileToBlob(TEXT("VS_Rough.cso"), &p_compiled_shader_code);
    }
    CHECK_FAILED(hr, 60);

    // create shader object
    hr = p_d_3d_dev->CreateVertexShader(p_compiled_shader_code->GetBufferPointer(),
                                        p_compiled_shader_code->GetBufferSize(), nullptr, &p_vertex_shader_);
    CHECK_FAILED(hr, 62);

    const INT error = create_input_layout(p_d_3d_dev, p_compiled_shader_code);
    CHECK_ERROR(error);

    safe_release<ID3DBlob>(p_compiled_shader_code);
    return 0;
}

INT material::create_pixel_shader(ID3D11Device* p_d_3d_dev, const shader shader)
{
    ID3DBlob* p_compiled_shader_code = nullptr;
    HRESULT hr = 0;

    // 2. load from already compiled shader
    if (shader == light)
    {
        hr = D3DReadFileToBlob(TEXT("LightPixelShader.cso"), &p_compiled_shader_code);
    }
    else if (shader == matt)
    {
        hr = D3DReadFileToBlob(TEXT("PS_Matt.cso"), &p_compiled_shader_code);
    }
    else if (shader == glossy)
    {
        hr = D3DReadFileToBlob(TEXT("PS_Glossy.cso"), &p_compiled_shader_code);
    }
    else if (shader == rough)
    {
        hr = D3DReadFileToBlob(TEXT("PS_Rough.cso"), &p_compiled_shader_code);
    }
    CHECK_FAILED(hr, 64);

    // create shader object
    hr = p_d_3d_dev->CreatePixelShader(p_compiled_shader_code->GetBufferPointer(),
                                       p_compiled_shader_code->GetBufferSize(), nullptr, &p_pixel_shader_);
    CHECK_FAILED(hr, 66);

    safe_release<ID3DBlob>(p_compiled_shader_code);

    return 0;
}

INT material::create_input_layout(ID3D11Device* p_d_3d_dev, ID3DBlob* p_vertex_shader_data)
{
    D3D11_INPUT_ELEMENT_DESC elements[5] = {};

    elements[0].SemanticName = "POSITION";
    elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;

    elements[1].SemanticName = "NORMAL";
    elements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

    elements[2].SemanticName = "TANGENT";
    elements[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    elements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    
    elements[3].SemanticName = "TEXCOORD";
    elements[3].Format = DXGI_FORMAT_R32G32_FLOAT;
    elements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

    elements[4].SemanticName = "COLOR";
    elements[4].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    elements[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

    const HRESULT hr = p_d_3d_dev->CreateInputLayout(elements, 5, p_vertex_shader_data->GetBufferPointer(),
                                                     p_vertex_shader_data->GetBufferSize(), &p_input_layout_);
    CHECK_FAILED(hr, 68);
    return 0;
}

INT material::create_matrix_buffer(ID3D11Device* p_d_3d_dev)
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(matrix_buffer);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    const HRESULT hr = p_d_3d_dev->CreateBuffer(&desc, nullptr, &p_matrix_buffer_);
    CHECK_FAILED(hr, 61);

    return 0;
}

INT material::create_frame_buffer(ID3D11Device* p_d_3d_dev)
{
    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(frame_buffer);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    const HRESULT hr = p_d_3d_dev->CreateBuffer(&desc, nullptr, &p_frame_buffer_);
    CHECK_FAILED(hr, 67);

    return 0;
}

void material::set_matrices(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT4X4* p_world_matrix,
                            const XMFLOAT4X4* p_view_matrix, const XMFLOAT4X4* p_projection_matrix) const
{
    const XMMATRIX w_matrix = XMLoadFloat4x4(p_world_matrix);
    const XMMATRIX view_matrix = XMLoadFloat4x4(p_view_matrix);
    const XMMATRIX projection_matrix = XMLoadFloat4x4(p_projection_matrix);

    const XMMATRIX wvp_matrix = XMMatrixTranspose(w_matrix * view_matrix * projection_matrix);

    D3D11_MAPPED_SUBRESOURCE data = {};
    const HRESULT hr = p_d_3d_dev_context->Map(p_matrix_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
    if (FAILED(hr)) return;

    const auto buffer = static_cast<matrix_buffer*>(data.pData);
    XMStoreFloat4x4(&(buffer->world_view_projection_matrix), wvp_matrix);
    XMStoreFloat4x4(&(buffer->world_matrix), XMMatrixTranspose(w_matrix));

    p_d_3d_dev_context->Unmap(p_matrix_buffer_, 0);

    p_d_3d_dev_context->VSSetConstantBuffers(0, 1, &p_matrix_buffer_);
}

void material::set_frame_buffer(ID3D11DeviceContext* p_d_3d_dev_context, const XMFLOAT3* p_camera_pos) const
{
    D3D11_MAPPED_SUBRESOURCE data = {};
    const HRESULT hr = p_d_3d_dev_context->Map(p_frame_buffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
    if (FAILED(hr)) return;

    const auto buffer = static_cast<frame_buffer*>(data.pData);
    XMStoreFloat3(&(buffer->camera_pos), XMLoadFloat3(p_camera_pos));

    p_d_3d_dev_context->Unmap(p_frame_buffer_, 0);

    p_d_3d_dev_context->VSSetConstantBuffers(1, 2, &p_frame_buffer_);
}

INT material::create_texture_sampler(ID3D11Device* p_d_3d_dev, const textures texture)
{
    LPCTSTR name;
    if (texture == wall)
    {
        name = TEXT("wall.jpg");
    }
    else if (texture == gray)
    {
        name = TEXT("gray.jpg");
    }
    else if (texture == floor)
    {
        name = TEXT("floor.jpg");
    }
    else if (texture == dragon)
    {
        name = TEXT("dragon.jpg");
    }
    else if (texture == solid)
    {
        name = TEXT("solid.jpg");
    }
    else
    {
        name = TEXT("wall.jpg");
    }

    // create texture
    HRESULT hr = CreateWICTextureFromFile(p_d_3d_dev, name, nullptr, &p_shader_resource_view_);
    CHECK_FAILED(hr, 63);

    //create sample
    D3D11_SAMPLER_DESC desc = {};
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    hr = p_d_3d_dev->CreateSamplerState(&desc, &p_sampler_state_);
    CHECK_FAILED(hr, 65);

    return 0;
}

INT material::create_normal_map_sampler(ID3D11Device* p_d_3d_dev, const textures texture)
{
    LPCTSTR name;
    if (texture == wall)
    {
        name = TEXT("wallnormal.png");
    }
    else if (texture == gray)
    {
        name = TEXT("graynormal.png");
    }
    else if (texture == floor)
    {
        name = TEXT("floornormal.jpg");
    }
    else if (texture == dragon)
    {
        name = TEXT("dragonnormal.png");
    }
    else if (texture == solid)
    {
        name = TEXT("solidnormal.png");
    }
    else 
    {
        name = TEXT("wallnormal.png");
    }

    // create texture
    const HRESULT hr = CreateWICTextureFromFile(p_d_3d_dev, name, nullptr, &p_shader_resource_view2_);
    CHECK_FAILED(hr, 69);

    return 0;
}
