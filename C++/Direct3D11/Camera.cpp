#include "Camera.h"

using namespace DirectX;

INT camera::init(const UINT screen_width, const UINT screen_height)
{
    const XMVECTOR position = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
    const XMMATRIX view_matrix = XMMatrixLookToLH(
        XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f), // camera pos
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), // camera forward vector
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)); // camera up vector

    XMStoreFloat4x4(&view_matrix_, view_matrix);
    XMStoreFloat3(&position_, position);

    // projection matrix
    const XMMATRIX projection_matrix = XMMatrixPerspectiveFovLH(
        XM_PI * 0.3333333f,
        static_cast<FLOAT>(screen_width) / screen_height, 0.3f,
        1000.0f);

    XMStoreFloat4x4(&projection_matrix_, projection_matrix);

    return 0;
}

void camera::update(const FLOAT dt)
{
    static FLOAT pos_x = 0.0f;
    static FLOAT pos_y = 0.0f;
    static FLOAT pos_z = -5.0f;

    static FLOAT move = 5.0f * dt;

    if (GetAsyncKeyState(VK_RIGHT)) pos_x += move;
    if (GetAsyncKeyState(VK_LEFT)) pos_x -= move;
    if (GetAsyncKeyState(VK_DOWN)) pos_y -= move;
    if (GetAsyncKeyState(VK_UP)) pos_y += move;
    if (GetAsyncKeyState(VK_DELETE)) pos_z -= move;
    if (GetAsyncKeyState(VK_END)) pos_z += move;
    
    const XMVECTOR position = XMVectorSet(pos_x, pos_y, pos_z, 0.0f);
    const XMMATRIX view_matrix = XMMatrixLookToLH(
        XMVectorSet(pos_x, pos_y, pos_z, 0.0f), // camera pos
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), // camera forward vector
        XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

    XMStoreFloat4x4(&view_matrix_, view_matrix);
    XMStoreFloat3(&position_, position);
}

void camera::de_init()
{
}
