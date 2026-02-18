#include "Graphics.h"
#include "Engine.h"

ComPtr<ID3D11Device> Graphics::device = nullptr;
ComPtr<ID3D11DeviceContext> Graphics::device_context = nullptr;

Graphics::Graphics() { }

Graphics::~Graphics() { }

void Graphics::Init(HWND hwnd, uint32 width, uint32 height)
{
	CreateDeviceAndSwapChain(hwnd, width, height);
	CreateRenderTargetView();
	CreateDepthStencilView(width, height);
	SetViewport(width, height);
}

void Graphics::RenderBegin()
{
	device_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());
	device_context->ClearRenderTargetView(render_target_view.Get(), clear_color);
	device_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	device_context->RSSetViewports(1, &viewport);
}

void Graphics::RenderEnd()
{
	HRESULT hr = swap_chain->Present(1, 0);
	assert(SUCCEEDED(hr));
}

void Graphics::CreateDeviceAndSwapChain(HWND hwnd, uint32 width, uint32 height)
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = hwnd;
	desc.Windowed = true;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		swap_chain.GetAddressOf(),
		device.GetAddressOf(),
		nullptr,
		device_context.GetAddressOf()
	);

	assert(SUCCEEDED(hr));
}

void Graphics::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, render_target_view.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Graphics::CreateDepthStencilView(uint32 width, uint32 height)
{
	HRESULT hr;

	D3D11_TEXTURE2D_DESC texture2d_desc = { 0 };
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;

	hr = device->CreateTexture2D(&texture2d_desc, nullptr, depth_stencil_texture.GetAddressOf());
	assert(SUCCEEDED(hr));
	
	D3D11_DEPTH_STENCIL_VIEW_DESC stencil_desc;
	ZeroMemory(&stencil_desc, sizeof(stencil_desc));
	stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencil_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencil_desc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depth_stencil_texture.Get(), &stencil_desc, depth_stencil_view.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void Graphics::SetViewport(uint32 width, uint32 height)
{
	viewport.TopLeftX = 0.f;
	viewport.TopLeftY = 0.f;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
}