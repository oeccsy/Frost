#pragma once
#include<vector>
#include<string>

class Object
{
public:
	Object();
	virtual ~Object();
	virtual void Update() = 0;
	virtual void Render() = 0;

protected:
	virtual void CreateGeometry() = 0;
	virtual void CreateVS() = 0;
	virtual void CreatePS() = 0;
	virtual void CreateInputLayout() = 0;
	void LoadShaderFromFile(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob);
	
protected:
	// Geometry
	vector<Vertex> _vertices;
	vector<uint32> _indices;
	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;

	// VS
	ComPtr<ID3D11VertexShader> _vertexShader = nullptr;
	ComPtr<ID3DBlob> _vsBlob = nullptr;

	// PS
	ComPtr<ID3D11PixelShader> _pixelShader = nullptr;
	ComPtr<ID3DBlob> _psBlob = nullptr;
};