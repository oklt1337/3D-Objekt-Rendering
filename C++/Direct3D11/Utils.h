#pragma once

#define CHECK_ERROR(error) if (error != 0) return error;
#define CHECK_NULL(obj, error) if (obj == nullptr) return error;
#define CHECK_FAILED(hr, error) if (FAILED(hr)) return error;

template <typename T>
void safe_release(T* & obj)
{
    if (obj != nullptr)
    {
        obj->Release();
        obj = nullptr;
    }
}

#define D_3D_COLOR_VALUE_RGB(r,g,b) (D3DCOLORVALUE) { r / 255.0f, g / 255.0f,  b / 255.0f, 1.0f }
