#pragma once
#include <d3d11.h>
#pragma comment (lib,"d3d11.lib")
class d_3d
{
public:
	INT init(HWND h_wnd, UINT width, UINT height, BOOL is_fullscreen);
	void begin_scene(FLOAT r, FLOAT g, FLOAT b) const;
	void end_scene() const;
	void de_init();

	ID3D11Device* get_device() const { return pd_3d_device_; }
	ID3D11DeviceContext* get_device_context() const { return pd_3d_device_context_; }

private:
	ID3D11Device* pd_3d_device_ = nullptr; // creating objects
	ID3D11DeviceContext* pd_3d_device_context_ = nullptr; // handling render pipeline
	IDXGISwapChain* dxgi_swap_chain_ = nullptr; // hold references to front- and back buffer and swap them
	ID3D11RenderTargetView* pd_3d_render_target_view_ = nullptr; // back buffer render target
	ID3D11DepthStencilView* p_depth_stencil_view_ = nullptr;
	ID3D11RasterizerState* p_rasterizer_state_ = nullptr;
};

