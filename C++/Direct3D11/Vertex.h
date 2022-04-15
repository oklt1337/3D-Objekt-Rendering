#pragma once
#include <d3d11.h>

using namespace DirectX;

struct vertex
{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT3 tangent;
    XMFLOAT2 uv;
    XMFLOAT4 color;

    vertex()
    {
        position = XMFLOAT3(0.0f, 0.0f, 0.0f);
        normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
        tangent = XMFLOAT3(0.0f, 0.0f, 0.0f);
        uv = XMFLOAT2(0.0f, 0.0f);
        color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    vertex(const FLOAT x, const FLOAT y, const FLOAT z) : position(x, y, z), normal(0.0f, 0.0f, 0.0f),
                                                          tangent(0.0f, 0.0f, 0.0f),
                                                          uv(0.0f, 0.0f),
                                                          color(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }

    vertex(const FLOAT x, const FLOAT y, const FLOAT z, const FLOAT r, const FLOAT g, const FLOAT b) :
        position(x, y, z), normal(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f),
        uv(0.0f, 0.0f), color(r, g, b, 1.0f)
    {
    }

    vertex(const FLOAT x, const FLOAT y, const FLOAT z, const UINT r, const UINT g, const UINT b) : position(x, y, z),
        normal(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f),
        uv(0.0f, 0.0f), color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f)
    {
    }

    vertex(const FLOAT x, const FLOAT y, const FLOAT z, const FLOAT u, const FLOAT v) : position(x, y, z),
        normal(0.0f, 0.0f, 0.0f), tangent(0.0f, 0.0f, 0.0f),
        uv(u, v), color(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }

    vertex(const FLOAT x, const FLOAT y, const FLOAT z, const FLOAT nx, const FLOAT ny, const FLOAT nz, const FLOAT u,
           const FLOAT v) : position(x, y, z), normal(nx, ny, nz), tangent(0.0f, 0.0f, 0.0f),
                            uv(u, v), color(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }

    vertex(const FLOAT x, const FLOAT y, const FLOAT z, const FLOAT nx, const FLOAT ny, const FLOAT nz, const FLOAT tx,
           const FLOAT ty, const FLOAT tz, const FLOAT u, const FLOAT v) : position(x, y, z), normal(nx, ny, nz),
                                                                           tangent(tx, ty, tz),
                                                                           uv(u, v), color(1.0f, 1.0f, 1.0f, 1.0f)
    {
    }
};
