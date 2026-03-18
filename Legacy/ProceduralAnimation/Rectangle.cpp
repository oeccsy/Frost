#include "pch.h"
#include "Object.h"
#include "Rectangle.h"
#include "RenderFramework.h"

Rectangle::Rectangle()
{
    CreateGeometry();
    CreateVS();
    CreatePS();
    CreateInputLayout();
    CreateSRV();
    CreateContantBuffer();
}

Rectangle::~Rectangle()
{
}

//void Rectangle::Update()
//{
//    // SRT
//    Matrix s = XMMatrixScalingFromVector(_localScale);
//    Matrix r = XMMatrixRotationRollPitchYawFromVector(_localRotation) * XMMatrixRotationZ(XMConvertToRadians(45.0f));
//    Matrix t = XMMatrixTranslationFromVector(_localPosition);
//
//    Matrix matWorld = s * r * t;
//    _transformData.matWorld = matWorld;
//
//    D3D11_MAPPED_SUBRESOURCE subResource;
//    ZeroMemory(&subResource, sizeof(subResource));
//
//    auto deviceContext = RenderFramework::GetDeviceContext();
//    deviceContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
//    ::memcpy(subResource.pData, &_transformData, sizeof(_transformData));
//    deviceContext->Unmap(_constantBuffer.Get(), 0);
//}
//
//void Rectangle::Render()
//{
//    uint32 stride = sizeof(Vertex);
//    uint32 offset = 0;
//
//    auto deviceContext = RenderFramework::GetDeviceContext();
//
//    // IA
//    deviceContext->IASetVertexBuffers(0, 1, _vertexBuffer.GetAddressOf(), &stride, &offset);
//    deviceContext->IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//    deviceContext->IASetInputLayout(_inputLayout.Get());
//    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//    // VS
//    deviceContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
//    deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
//
//    // RS
//
//    // PS
//    deviceContext->PSSetShader(_pixelShader.Get(), nullptr, 0);
//    deviceContext->PSSetShaderResources(0, 1, _shaderResourceView_smile.GetAddressOf());
//    deviceContext->PSSetShaderResources(1, 1, _shaderResourceView_stars.GetAddressOf());
//
//    // OM
//    deviceContext->DrawIndexed(_indices.size(), 0, 0);
//}

void Rectangle::CreateGeometry()
{
    _vertices.resize(4);

    _vertices[0].position = Vec3(0.2f, 0.2f, 0);
    _vertices[0].uv = Vec2(1.f, 0.f);
    _vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
    _vertices[1].position = Vec3(-0.2f, 0.2f, 0);
    _vertices[1].uv = Vec2(0.f, 0.f);
    _vertices[1].color = Color(0.f, 1.f, 0.f, 1.f);
    _vertices[2].position = Vec3(-0.2f, -0.2f, 0);
    _vertices[2].uv = Vec2(0.f, 1.f);
    _vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);
    _vertices[3].position = Vec3(0.2f, -0.2f, 0);
    _vertices[3].uv = Vec2(1.f, 1.f);
    _vertices[3].color = Color(0.f, 0.f, 1.f, 1.f);

    _indices = { 0, 2, 1, 0, 3, 2 };

    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.ByteWidth = (uint32)(sizeof(Vertex) * _vertices.size());

        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(data));
        data.pSysMem = _vertices.data();

        auto device = Graphics::GetDevice();
        HRESULT hr = device->CreateBuffer(&desc, &data, _vertexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Usage = D3D11_USAGE_IMMUTABLE;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.ByteWidth = (uint32)(sizeof(Vertex) * _indices.size());

        D3D11_SUBRESOURCE_DATA data;
        ZeroMemory(&data, sizeof(data));
        data.pSysMem = _indices.data();

        auto device = Graphics::GetDevice();
        HRESULT hr = device->CreateBuffer(&desc, &data, _indexBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

void Rectangle::CreateVS()
{
    LoadShaderFromFile(L"Triangle.hlsl", "VS", "vs_5_0", _vsBlob);

    auto device = Graphics::GetDevice();
    HRESULT hr = device->CreateVertexShader(_vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf());
    assert(SUCCEEDED(hr));
}

void Rectangle::CreatePS()
{
    LoadShaderFromFile(L"Triangle.hlsl", "PS", "ps_5_0", _psBlob);

    auto device = Graphics::GetDevice();
    HRESULT hr = device->CreatePixelShader(_psBlob->GetBufferPointer(), _psBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf());
    assert(SUCCEEDED(hr));
}

void Rectangle::CreateInputLayout()
{
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
       {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
       {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
       {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
    auto device = Graphics::GetDevice();
    device->CreateInputLayout(layout, count, _vsBlob->GetBufferPointer(), _vsBlob->GetBufferSize(), _inputLayout.GetAddressOf());
}

void Rectangle::CreateSRV()
{
    {
        TexMetadata md;
        ScratchImage img;
        HRESULT hr = LoadFromWICFile(L"Smile.png", WIC_FLAGS_NONE, &md, img);
        assert(SUCCEEDED(hr));

        auto device = Graphics::GetDevice();
        hr = CreateShaderResourceView(device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView_smile.GetAddressOf());
        assert(SUCCEEDED(hr));
    }

    {
        TexMetadata md;
        ScratchImage img;
        HRESULT hr = LoadFromWICFile(L"Stars.png", WIC_FLAGS_NONE, &md, img);
        assert(SUCCEEDED(hr));

        auto device = Graphics::GetDevice();
        hr = CreateShaderResourceView(device.Get(), img.GetImages(), img.GetImageCount(), md, _shaderResourceView_stars.GetAddressOf());
        assert(SUCCEEDED(hr));
    }
}

void Rectangle::CreateContantBuffer()
{
    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Usage = D3D11_USAGE_DYNAMIC; // CPU Write + GPU Read
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.ByteWidth = sizeof(TransformData);
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; 

    auto device = Graphics::GetDevice();
    HRESULT hr = device->CreateBuffer(&desc, nullptr, _constantBuffer.GetAddressOf());
    assert(SUCCEEDED(hr));
}
