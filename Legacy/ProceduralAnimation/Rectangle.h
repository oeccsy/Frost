#pragma once
class Object;

class Rectangle : public Object
{
public:
	Rectangle();
	~Rectangle() override;
	//void Update();
	//void Render();

protected:
	void CreateGeometry();
	void CreateVS();
	void CreatePS();
	void CreateInputLayout();
	void CreateSRV();
	void CreateContantBuffer();

private:
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView_smile = nullptr;
	ComPtr<ID3D11ShaderResourceView> _shaderResourceView_stars = nullptr;
	
	TransformData _transformData;
	ComPtr<ID3D11Buffer> _constantBuffer;
};