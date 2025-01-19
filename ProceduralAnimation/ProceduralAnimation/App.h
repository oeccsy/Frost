#pragma once

class App
{
public:
	App();
	~App();

public:
	void Init(HWND hwnd);
	int Run();
	void LoadScene(class Scene* newScene);

private:
	HWND _hwnd;
	class Scene* currentScene;

	shared_ptr<Graphics> _graphics;
	shared_ptr<Pipeline> _pipeline;



//private:
//	shared_ptr<Geometry<VertexTextureData>> _geometry;
//	ComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
//	ComPtr<ID3D11Buffer> _indexBuffer = nullptr;
//	ComPtr<ID3D11InputLayout> _inputLayout = nullptr;
//
//	shared_ptr<VertexShader> _vertexShader;
//	shared_ptr<PixelShader> _pixelShader;
//	ComPtr<ID3D11RasterizerState> _rasterizerState;
//	ComPtr<ID3D11ShaderResourceView> _texture;
//	ComPtr<ID3D11SamplerState> _samplerState;
//	ComPtr<ID3D11BlendState> _blendState;
//
//private:
//	TransformData _transformData;
//	ComPtr<ID3D11Buffer> _constantBuffer;
//
//	Vec3 _localPosition = { 0.f, 0.f, 0.f };
//	Vec3 _localRotation = { 0.f, 0.f, 0.f };
//	Vec3 _localScale = { 1.f, 1.f, 1.f };
};