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

#include <d3dcompiler.h>
#include <iostream>

#include "Graphics/Shader/DXShader.h"

bool Jade::Graphics::DXShader::Create()
{
	HRESULT shaderResult = 0;

	switch (type)
	{
	case ShaderType::Compute:
		ID3D11ComputeShader* l_pComputeShader;
		shaderResult = device->m_pDevice->CreateComputeShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &l_pComputeShader);
		break;
	case ShaderType::Domain:
		ID3D11DomainShader* l_pDomainShader;
		shaderResult = device->m_pDevice->CreateDomainShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &l_pDomainShader);
		break;
	case ShaderType::Geometry:
		ID3D11GeometryShader* l_pGeometryShader;
		shaderResult = device->m_pDevice->CreateGeometryShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &l_pGeometryShader);
		break;
	case ShaderType::Pixel:
		ID3D11PixelShader* l_pPixelShader;
		shaderResult = device->m_pDevice->CreatePixelShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &l_pPixelShader);

		device->m_pImmediateContext->PSSetShader(l_pPixelShader, nullptr, 0);

		break;
	case ShaderType::Tesselation:
		ID3D11HullShader* l_pHullShader;
		shaderResult = device->m_pDevice->CreateHullShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &l_pHullShader);
		break;
	case ShaderType::Vertex:
		ID3D11VertexShader* l_pVertexShader;
		shaderResult = device->m_pDevice->CreateVertexShader(m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), nullptr, &l_pVertexShader);

		// Define the input layout
		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		HRESULT inputResult = device->m_pDevice->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout), m_pShaderBlob->GetBufferPointer(), m_pShaderBlob->GetBufferSize(), &m_pVertexLayout);

		device->m_pImmediateContext->VSSetShader(l_pVertexShader, nullptr, 0);

		break;
	}

	m_pShaderBlob->Release();

	return true;
}

bool Jade::Graphics::DXShader::Release()
{
	m_pVertexLayout->Release();

	if (m_pVertexLayout == nullptr)
		return true;

	return false;
}

bool Jade::Graphics::DXShader::Compile()
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	flags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	flags |= D3DCOMPILE_SKIP_OPTIMIZATION;

#endif

	// Prefer higher shader profile when possible as 5.0 provides better performance on 11-class hardware.
	LPCSTR profile = "";
	
	D3D_FEATURE_LEVEL featureLevel = device->m_pDevice->GetFeatureLevel();

	// Setting correct profile for the shader compilation.
	switch(type)
	{
	case ShaderType::Compute:
		profile = (featureLevel >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";
		break;
	case ShaderType::Domain:
		profile = (featureLevel >= D3D_FEATURE_LEVEL_11_0) ? "ds_5_0" : "ds_4_0";
		break;
	case ShaderType::Geometry:
		profile = (featureLevel >= D3D_FEATURE_LEVEL_11_0) ? "gs_5_0" : "gs_4_0";
		break;
	case ShaderType::Pixel:
		profile = (featureLevel >= D3D_FEATURE_LEVEL_11_0) ? "ps_5_0" : "ps_4_0";
		break;
	case ShaderType::Tesselation:
		profile = (featureLevel >= D3D_FEATURE_LEVEL_11_0) ? "hs_5_0" : "hs_4_0";
		break;
	case ShaderType::Vertex:
		profile = (featureLevel >= D3D_FEATURE_LEVEL_11_0) ? "vs_5_0" : "vs_4_0";
		break;
	}

	if(profile != "")
	{
		ID3DBlob* l_pErrorBlob = nullptr;

		// Compile the shader
		HRESULT hr = D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "Main", profile, flags, 0, &m_pShaderBlob, &l_pErrorBlob);

		if (FAILED(hr))
		{
			// Output compilation errors to console.
			std::cout << "Shader Compilation Error: " << static_cast<char*>(l_pErrorBlob->GetBufferPointer()) << std::endl;

			// Release loose pointers.
			l_pErrorBlob->Release();

			return false;
		}

		l_pErrorBlob->Release();

		return true;
	}

	return false;
}
