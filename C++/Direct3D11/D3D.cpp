#include "D3D.h"

#include <d3d9types.h>
#include <xutility>

#include "Utils.h"

INT d_3d::init(const HWND h_wnd, const UINT width, const UINT height, const BOOL is_fullscreen)
{
    // 1. describe creation process
    DXGI_SWAP_CHAIN_DESC dxgi_swap_chain_desc = {};
    dxgi_swap_chain_desc.OutputWindow = h_wnd;
    dxgi_swap_chain_desc.Windowed = !is_fullscreen;
    dxgi_swap_chain_desc.BufferCount = 1;
    dxgi_swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    dxgi_swap_chain_desc.BufferDesc.Width = width;
    dxgi_swap_chain_desc.BufferDesc.Height = height;
    dxgi_swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dxgi_swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    dxgi_swap_chain_desc.SampleDesc.Count = 1;

    // 2. create Direct3D 11 device, device context & swap chain
    constexpr D3D_FEATURE_LEVEL supported_feature_level[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    D3D_FEATURE_LEVEL chosen_level = {};

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, supported_feature_level,
        std::size(supported_feature_level), D3D11_SDK_VERSION,
        &dxgi_swap_chain_desc,
        &dxgi_swap_chain_, &pd_3d_device_, &chosen_level,
        &pd_3d_device_context_);
    CHECK_FAILED(hr, 20);

    // 3. create render target view
    ID3D11Texture2D* p_back_buffer_texture = nullptr;
    dxgi_swap_chain_->GetBuffer(0, IID_PPV_ARGS(&p_back_buffer_texture));
    CHECK_FAILED(hr, 22);

    hr = pd_3d_device_->CreateRenderTargetView(p_back_buffer_texture, nullptr, &pd_3d_render_target_view_);
    CHECK_FAILED(hr, 24);

    safe_release<ID3D11Texture2D>(p_back_buffer_texture);

    // 4. create depth stencil view
    ID3D11Texture2D* p_depth_stencil_texture = nullptr;
    D3D11_TEXTURE2D_DESC depth_stencil_texture_desc = {};
    depth_stencil_texture_desc.Width = width;
    depth_stencil_texture_desc.Height = height;
    depth_stencil_texture_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_texture_desc.ArraySize = 1;
    depth_stencil_texture_desc.SampleDesc.Count = 1;

    hr = pd_3d_device_->CreateTexture2D(&depth_stencil_texture_desc, nullptr, &p_depth_stencil_texture);
    CHECK_FAILED(hr, 26);

    hr = pd_3d_device_->CreateDepthStencilView(p_depth_stencil_texture, nullptr, &p_depth_stencil_view_);
    CHECK_FAILED(hr, 28);
    
    safe_release<ID3D11Texture2D>(p_depth_stencil_texture);
    
    // 5. create rasterizer state
    D3D11_RASTERIZER_DESC rasterizer_desc = {};
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_BACK;

    hr = pd_3d_device_->CreateRasterizerState(&rasterizer_desc, &p_rasterizer_state_);
    CHECK_FAILED(hr, 21);
    
    // 6. set viewport
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = width;
    viewport.Height = height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    // 7. setup render pipeline
    pd_3d_device_context_->OMSetRenderTargets(1, &pd_3d_render_target_view_, p_depth_stencil_view_);
    pd_3d_device_context_->RSSetViewports(1, &viewport);
    pd_3d_device_context_->RSSetState(p_rasterizer_state_);

    return 0;
}

void d_3d::begin_scene(const FLOAT r, const FLOAT g, const FLOAT b) const
{
    // clear backbuffer
    const FLOAT background_color[] = {r, g, b, 1.0f};
    pd_3d_device_context_->ClearRenderTargetView(pd_3d_render_target_view_, background_color);

    // clear depth buffer
    pd_3d_device_context_->ClearDepthStencilView(p_depth_stencil_view_, D3D11_CLEAR_DEPTH, 1.0f, 0xffffffff);
}

void d_3d::end_scene() const
{
    //swap front with back buffer
    dxgi_swap_chain_->Present(0, 0);
}

void d_3d::de_init()
{
    safe_release<ID3D11RasterizerState>(p_rasterizer_state_);
    safe_release<ID3D11DepthStencilView>(p_depth_stencil_view_);
    safe_release<ID3D11Device>(pd_3d_device_);
    safe_release<ID3D11DeviceContext>(pd_3d_device_context_);
    safe_release<IDXGISwapChain>(dxgi_swap_chain_);
    safe_release<ID3D11RenderTargetView>(pd_3d_render_target_view_);
}
