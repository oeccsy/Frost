#include "pch.h"
#include "RenderFramework.h"
#include "Scene.h"

RenderFramework::RenderFramework() {}

RenderFramework::~RenderFramework() {}

void RenderFramework::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_width = GWinSizeX;
	_height = GWinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

int RenderFramework::Run()
{
	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			RenderBegin();

			if (currentScene)
			{
				currentScene->Update();
				currentScene->Render();
			}
			
			RenderEnd();
		}
	}

	return (int)msg.wParam;
}

void RenderFramework::LoadScene(Scene* newScene)
{
	if (currentScene) delete currentScene;
	currentScene = newScene;
	currentScene->Init();
}

ComPtr<ID3D11Device> RenderFramework::_device = nullptr;
ComPtr<ID3D11DeviceContext> RenderFramework::_deviceContext = nullptr;

ID3D11Device* RenderFramework::GetDevice()
{
	return _device.Get();
}

ID3D11DeviceContext* RenderFramework::GetDeviceContext()
{
	return _deviceContext.Get();
}

void RenderFramework::RenderBegin()
{
	_deviceContext->OMSetRenderTargets(1, _renderTargetView.GetAddressOf(), nullptr);
	_deviceContext->ClearRenderTargetView(_renderTargetView.Get(), _clearColor);
	_deviceContext->RSSetViewports(1, &_viewport);
}

void RenderFramework::RenderEnd()
{
	HRESULT hr = _swapChain->Present(1, 0);
	assert(SUCCEEDED(hr));
}

void RenderFramework::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = _width;
		desc.BufferDesc.Height = _height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.BufferCount = 1;
		desc.OutputWindow = _hwnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		nullptr,
		_deviceContext.GetAddressOf()
	);

	assert(SUCCEEDED(hr));
}

void RenderFramework::CreateRenderTargetView()
{
	HRESULT hr;

	ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	_device->CreateRenderTargetView(backBuffer.Get(), nullptr, _renderTargetView.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void RenderFramework::SetViewport()
{
	_viewport.TopLeftX = 0.f;
	_viewport.TopLeftY = 0.f;
	_viewport.Width = static_cast<float>(_width);
	_viewport.Height = static_cast<float>(_height);
	_viewport.MinDepth = 0.f;
	_viewport.MaxDepth = 1.f;
}