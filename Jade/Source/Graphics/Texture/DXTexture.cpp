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

#ifdef _WIN32

#include "DXTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool Jade::Graphics::DXTexture::Bind()
{
	// We need to direct path to the location of the texture from our executable.
	std::string filepath = ".\\resources\\models\\";
	filepath.append(filename.c_str());

	int width;
	int height;
	int bpp;
	BYTE* bits = stbi_load(filepath.c_str(), &width, &height, &bpp, STBI_rgb_alpha);

	// Create our texture description.
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
 
	// Pass our bitmap data.
	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(subData));
	subData.pSysMem = bits;
	subData.SysMemPitch = width * bpp;
	
	// Create the texture;
	HRESULT hr = device->GetID3D11Device()->CreateTexture2D(&texDesc, &subData, m_pTexture.GetAddressOf());

	if(FAILED(hr))
	{
		std::cout << "Texture2D failed to be created..." << std::endl;
		return false;
	}

	// We now need the resource description for our texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC resDesc;
	ZeroMemory(&resDesc, sizeof(resDesc));
	resDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resDesc.Format = texDesc.Format;
	resDesc.Texture2D.MipLevels = texDesc.MipLevels;

	// Create a resource view.
	hr = device->GetID3D11Device()->CreateShaderResourceView(m_pTexture.Get(), &resDesc, m_pShaderResourceView.GetAddressOf());

	if (FAILED(hr))
	{
		std::cout << "ShaderResourceView failed to be created..." << std::endl;
		return false;
	}

	// Generate our mip maps.
	device->GetID3D11DeviceContext()->GenerateMips(m_pShaderResourceView.Get());

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	// Lastly create our sampler state.
	hr = device->GetID3D11Device()->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf());

	if (FAILED(hr))
	{
		std::cout << "SamplerState failed to be created..." << std::endl;
		return false;
	}

	// We are now done with stb_image, unload to prevent leaks.
	stbi_image_free(bits);

	device->GetID3D11DeviceContext()->PSSetShaderResources(0, 1, m_pShaderResourceView.GetAddressOf());
	device->GetID3D11DeviceContext()->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());

	std::cout << "Texture " << filename << " was created successfully..." << std::endl;

	return true;
}

bool Jade::Graphics::DXTexture::Unbind()
{
	std::cout << "Texture " << filename << " was released successfully..." << std::endl;

	return true;
}

ComPtr<ID3D11ShaderResourceView>& Jade::Graphics::DXTexture::GetID3D11ShaderResourceView()
{
	return m_pShaderResourceView;
}

ComPtr<ID3D11SamplerState>& Jade::Graphics::DXTexture::GetID3D11SamplerState()
{
	return m_pSamplerState;
}

#endif // _WIN32