#pragma once

enum class BufferType
{
    Vertex,
    Index
};

class BufferPool
{
public:
    BufferPool(BufferType bufferType, size_t bufferSize, size_t poolSize);
    virtual ~BufferPool();

public:
    ComPtr<ID3D11Buffer> GetBuffer();
    void ReturnBuffer(ComPtr<ID3D11Buffer> buffer);

private:
    ComPtr<ID3D11Buffer> CreateBuffer(BufferType bufferType, size_t bufferSize);

public:
    static unique_ptr<BufferPool> vertexBufferPool;
    static unique_ptr<BufferPool> indexBufferPool;

private:
    BufferType _bufferType;
    
    size_t _bufferSize;
    size_t _poolSize;

    vector<ComPtr<ID3D11Buffer>> _allBuffers;
    vector<ComPtr<ID3D11Buffer>> _freeBuffers;
};