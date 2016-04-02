/*
The MIT License (MIT)

Copyright (c) 2015 Ben Brown

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <iostream>

#include "Graphics/Device/DXDevice.h"

bool Jade::Graphics::DXDevice::Create()
{
	std::cout << "[Device Context]" << std::endl;

	HRESULT hr;

	unsigned int createDeviceFlags = 0;

#ifdef _DEBUG
	// Add the debug device flag along with any other defined flags.
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	unsigned int numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
	};

	unsigned int numFeatureLevels = ARRAYSIZE(featureLevels);

	if (window->Handle() == nullptr)
		return false;

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = specification.backBufferWidth;
	sd.BufferDesc.Height = specification.backBufferHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = static_cast<HWND>(window->Handle());
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, 
		numFeatureLevels, D3D11_SDK_VERSION, &sd, m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), &m_FeatureLevel, m_pImmediateContext.GetAddressOf());

	if (FAILED(hr))
		return false;

	ID3D11Texture2D* m_pBackBuffer = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&m_pBackBuffer));
	hr = m_pDevice->CreateRenderTargetView(m_pBackBuffer, nullptr, m_pRenderTargetView.GetAddressOf());

	if (FAILED(hr))
		return false;

	// Release object now that it is unneeded.
	m_pBackBuffer->Release();

	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(depthDesc));
	depthDesc.Width = window->GetWidth();
	depthDesc.Height = window->GetHeight();
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&depthDesc, nullptr, m_pDepthStencil.GetAddressOf());

	if (FAILED(hr))
		return false;
	
	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));
	stencilDesc.Format = depthDesc.Format;
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilDesc.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencil.Get(), &stencilDesc, m_pDepthStencilView.GetAddressOf());

	if (FAILED(hr))
		return false;

	m_pImmediateContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.Width = static_cast<float>(window->GetWidth());
	vp.Height = static_cast<float>(window->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_pImmediateContext->RSSetViewports(1, &vp);

	std::cout << "DirectX device was created successfully..." << std::endl;

	return true; // Successful.
}

bool Jade::Graphics::DXDevice::Release()
{
	std::cout << "DirectX device was disposed of successfully..." << std::endl;

	return true;
}

void Jade::Graphics::DXDevice::Clear(Math::Color color)
{	
	float colorRGBA[] = { color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha() };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), colorRGBA);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

// Swaps the front and back buffers.
void Jade::Graphics::DXDevice::Present()
{
	m_pSwapChain->Present(specification.vsync ? 1 : 0, 0);
}

char* Jade::Graphics::DXDevice::DeviceInformation()
{
	return nullptr;
}

std::shared_ptr<Jade::System::IWindow> Jade::Graphics::DXDevice::GetIWindow() const
{
	return window;
}

D3D_DRIVER_TYPE Jade::Graphics::DXDevice::GetD3DDriverType() const
{
	return m_DriverType;
}

D3D_FEATURE_LEVEL Jade::Graphics::DXDevice::GetD3DFeatureLevel() const
{
	return m_FeatureLevel;
}

D3D11_VIEWPORT Jade::Graphics::DXDevice::GetD3D11Viewport() const
{
	return m_Viewport;
}

const ComPtr<ID3D11Device>& Jade::Graphics::DXDevice::GetID3D11Device() const
{
	return m_pDevice;
}

const ComPtr<ID3D11DeviceContext>& Jade::Graphics::DXDevice::GetID3D11DeviceContext() const
{
	return m_pImmediateContext;
}

const ComPtr<IDXGISwapChain>& Jade::Graphics::DXDevice::GetIDXGISwapChain() const
{
	return m_pSwapChain;
}

const ComPtr<ID3D11RenderTargetView>& Jade::Graphics::DXDevice::GetID3D11RenderTargetView() const
{
	return m_pRenderTargetView;
}

const ComPtr<ID3D11Texture2D>& Jade::Graphics::DXDevice::GetID3D11DepthStencil() const
{
	return m_pDepthStencil;
}

const ComPtr<ID3D11DepthStencilView>& Jade::Graphics::DXDevice::GetID3D11DepthStencilView() const
{
	return m_pDepthStencilView;
}