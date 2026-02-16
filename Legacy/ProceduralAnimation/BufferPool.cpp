#include "pch.h"
#include "BufferPool.h"

BufferPool::BufferPool(BufferType bufferType, size_t bufferSize, size_t poolSize) : _bufferType(bufferType), _bufferSize(bufferSize), _poolSize(poolSize)
{
    for (size_t i = 0; i < poolSize; i++)
    {
        CreateBuffer(_bufferType, _bufferSize);
    }
}

BufferPool::~BufferPool()
{
    for (auto buffer : _allBuffers)
    {
        buffer->Release();
    }
}

ComPtr<ID3D11Buffer> BufferPool::GetBuffer()
{
    if (!_freeBuffers.empty())
    {
        ComPtr<ID3D11Buffer> buffer = _freeBuffers.back();
        _freeBuffers.pop_back();
        
        return buffer;
    }
    else
    {
        ComPtr<ID3D11Buffer> buffer = CreateBuffer(_bufferType, _bufferSize);
        _freeBuffers.pop_back();
        
        return buffer;
    }
}

void BufferPool::ReturnBuffer(ComPtr<ID3D11Buffer> buffer)
{
    _freeBuffers.push_back(buffer);
}

ComPtr<ID3D11Buffer> BufferPool::CreateBuffer(BufferType bufferType, size_t bufferSize)
{
    auto device = Graphics::GetDevice();

    {
        D3D11_BUFFER_DESC desc;
        ZeroMemory(&desc, sizeof(desc));
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = (bufferType == BufferType::Vertex) ? D3D11_BIND_VERTEX_BUFFER : D3D11_BIND_INDEX_BUFFER;
        desc.ByteWidth = static_cast<UINT>(bufferSize);
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        ComPtr<ID3D11Buffer> newBuffer;

        HRESULT hr = device->CreateBuffer(&desc, nullptr, newBuffer.GetAddressOf());
        assert(SUCCEEDED(hr));

        _allBuffers.push_back(newBuffer);
        _freeBuffers.push_back(newBuffer);

        return newBuffer;
    }
}

unique_ptr<BufferPool> BufferPool::vertexBufferPool = nullptr;
unique_ptr<BufferPool> BufferPool::indexBufferPool = nullptr;
