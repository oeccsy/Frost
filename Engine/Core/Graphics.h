#pragma once
#include "Core.h"

class ENGINE_API Graphics
{
public:
	Graphics(HWND hwnd, int width, int height);
	~Graphics();

	void RenderBegin();
	void RenderEnd();

	static ComPtr<ID3D11Device>& GetDevice() { return device; }
	static ComPtr<ID3D11DeviceContext>& GetDeviceContext() { return device_context; }

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void SetViewport();

private:
	HWND hwnd;
	int width;
	int height;

	ComPtr<IDXGISwapChain> swap_chain;
	ComPtr<ID3D11RenderTargetView> render_target_view;
	ComPtr<ID3D11Texture2D> depth_stencil_texture;
	ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	D3D11_VIEWPORT viewport = { 0 };
	float clear_color[4] = { 0.1f, 0.1f, 0.1f, 1.f };

	static ComPtr<ID3D11Device> device;
	static ComPtr<ID3D11DeviceContext> device_context;
};
