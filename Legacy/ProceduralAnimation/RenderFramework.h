#pragma once
#include <string>

class RenderFramework
{
public:
	RenderFramework();
	~RenderFramework();

public:
	void Init(HWND hwnd);
	int Run();
	void LoadScene(class Scene* newScene);

public:
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDeviceContext();

private:
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

private:
	HWND _hwnd;
	uint32 _width = 0;
	uint32 _height = 0;
	class Scene* currentScene;

private:
	// Device & SwapChain
	static ComPtr<ID3D11Device> _device;
	static ComPtr<ID3D11DeviceContext> _deviceContext;
	ComPtr<IDXGISwapChain> _swapChain = nullptr;

private :
	// Render Target View
	ComPtr<ID3D11RenderTargetView> _renderTargetView;

	// Misc
	D3D11_VIEWPORT _viewport = { 0 };
	float _clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.f };
};