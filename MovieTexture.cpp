#include "MovieTexture.h"

#define SAFE_RELEASE(x) if(x) { (x)->Release(); x = nullptr; }


bool MovieTexture::create(uint32_t new_xres, uint32_t new_yres, DXGI_FORMAT new_format, bool is_dynamic) {
    xres = new_xres;
    yres = new_yres;
    format = new_format;
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = xres;
    desc.Height = yres;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    if (is_dynamic) {
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    ID3D11Texture2D* tex2d = nullptr;
    HRESULT hr = GetDevice()->CreateTexture2D(&desc, nullptr, (ID3D11Texture2D**)&texture);
    if (FAILED(hr))
        return false;

    // Create a resource view so we can use the data in a shader
    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    ZeroMemory(&srv_desc, sizeof(srv_desc));
    srv_desc.Format = new_format;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MipLevels = desc.MipLevels;
    hr = GetDevice()->CreateShaderResourceView(texture, &srv_desc, &shader_resource_view);
    if (FAILED(hr))
        return false;

    return true;
}

void MovieTexture::activate(int slot) const {
    GetDeviceContext()->PSSetShaderResources(slot, 1, &shader_resource_view);
}

void MovieTexture::destroy() {
    SAFE_RELEASE(texture);
    SAFE_RELEASE(shader_resource_view);
}

bool MovieTexture::updateFromIYUV(const uint8_t* data, size_t data_size) {
    assert(data);
    D3D11_MAPPED_SUBRESOURCE ms;
    HRESULT hr = GetDeviceContext()->Map(texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
    if (FAILED(hr))
        return false;

    uint32_t bytes_per_texel = 1;
    assert(format == DXGI_FORMAT_R8_UNORM);
    assert(data_size == xres * yres * 3 / 4);

    const uint8_t* src = data;
    uint8_t* dst = (uint8_t*)ms.pData;

    // Copy the Y lines
    uint32_t nlines = yres / 2;
    uint32_t bytes_per_row = xres * bytes_per_texel;
    for (uint32_t y = 0; y < nlines; ++y) {
        memcpy(dst, src, bytes_per_row);
        src += bytes_per_row;
        dst += ms.RowPitch;
    }

    // Now the U and V lines, need to add Width/2 pixels of padding between each line
    uint32_t uv_bytes_per_row = bytes_per_row / 2;
    for (uint32_t y = 0; y < nlines; ++y) {
        memcpy(dst, src, uv_bytes_per_row);
        src += uv_bytes_per_row;
        dst += ms.RowPitch;
    }

    GetDeviceContext()->Unmap(texture, 0);
    return true;
}
