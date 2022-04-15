#include "light.h"
#include "Utils.h"

INT light::init(ID3D11Device* p_d_3d_dev, const light_data &light, const INT id)
{
    light_ = light;
    id_ = id;

    D3D11_BUFFER_DESC desc = {};
    desc.ByteWidth = sizeof(light_data);
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.Usage = D3D11_USAGE_IMMUTABLE;

    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = &light;

    const HRESULT hr = p_d_3d_dev->CreateBuffer(&desc, &data, &p_light_buffer_);
    CHECK_FAILED(hr, 70);
    
    return 0;
}

void light::render(ID3D11DeviceContext* p_d_3d_dev_context) const
{
    p_d_3d_dev_context->PSSetConstantBuffers(id_, 1, &p_light_buffer_);
}

void light::de_init()
{
    safe_release<ID3D11Buffer>(p_light_buffer_);
}
