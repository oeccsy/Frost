#pragma once
class Object;

class Frost : public Object
{
public:
	Frost();
	~Frost() override;
	void Update() override;
	void Render() override;

protected:
	void CreateGeometry() override;
	void CreateVS() override;
	void CreateGS();
	void CreatePS() override;
	void CreateInputLayout() override;

private:
	ComPtr<ID3D11GeometryShader> _geometryShader = nullptr;
	ComPtr<ID3DBlob> _gsBlob = nullptr;
};