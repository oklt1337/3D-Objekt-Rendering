#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class camera
{
public:
	INT init(UINT screen_width, UINT screen_height);
	void update(FLOAT dt);
	static void de_init();

	XMFLOAT4X4* get_view_matrix() { return &view_matrix_; };
	XMFLOAT4X4* get_projection_matrix() { return &projection_matrix_; }
	XMFLOAT3* get_camera_position() { return &position_; }

private:
	XMFLOAT4X4 view_matrix_ = {};
	XMFLOAT4X4 projection_matrix_ = {};
	XMFLOAT3 position_ = {};
};

