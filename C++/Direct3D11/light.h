#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class light
{
public:
    struct light_data
    {
        XMFLOAT3 light_direction;
        FLOAT light_intensity;
        XMFLOAT4 light_diffuse_color;
        XMFLOAT4 ambient_color;
        XMFLOAT4 specular_color;
        FLOAT specular_power;
        FLOAT glossy;
        XMFLOAT4 self_illumination_color;
        XMFLOAT2 buffer;
    };
    
    INT init(ID3D11Device* p_d_3d_dev, const light_data &light, INT id = 0);
    void render(ID3D11DeviceContext* p_d_3d_dev_context) const;
    void de_init();

private:
    INT id_ = 0;
    light_data light_ = {};
    ID3D11Buffer* p_light_buffer_ = nullptr;
};
