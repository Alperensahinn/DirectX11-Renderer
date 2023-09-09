#include "D3D11Graphics.h"
#include "GraphicsError.h"
#include "Macros.h"


#define STB_IMAGE_IMPLEMENTATION    
#include "3rdparty\stb_image.h"


#include <iostream>
#include <d3dcompiler.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


D3D11Graphics::D3D11Graphics(HWND hWnd)
{
	CheckerToken chk = {};

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_1;
	UINT               numLevelsRequested = 1;
	D3D_FEATURE_LEVEL  FeatureLevelsSupported;

	D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		&FeatureLevelsRequested,
		numLevelsRequested,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pd3dDevice,
		&FeatureLevelsSupported,
		&pImmediateContext
	) >> chk;

	// Create a render-target view
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBuffer) >> chk;
	pd3dDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), NULL));

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = 1600;
	vp.Height = 900;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	CHECK_INFOQUEUE( pImmediateContext->RSSetViewports(1, &vp) );

	CHECK_INFOQUEUE( pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );
}

void D3D11Graphics::Draw()
{
	CheckerToken chk = {};


	//vertex buffer-----------------------------------------------------------------------------------------------------------------------------------------
	const float vertices[] =
	{
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,	0.0f, 1.0f,
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	unsigned int stride = sizeof(float) * 5;
	unsigned int offset = 0u;

	D3D11_BUFFER_DESC vbd = {};
	vbd.ByteWidth = sizeof(vertices);
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.StructureByteStride = stride;

	D3D11_SUBRESOURCE_DATA vsd = {};
	vsd.pSysMem = vertices;

	pd3dDevice->CreateBuffer(&vbd, &vsd, &pVertexBuffer) >> chk;
	
	CHECK_INFOQUEUE( pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset) );


	//index buffer-----------------------------------------------------------------------------------------------------------------------------------------
	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

	D3D11_BUFFER_DESC ibd = {};
	ibd.ByteWidth = sizeof(indices);
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.StructureByteStride = sizeof(unsigned int) * 3;

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	pd3dDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u) );


	//vertex shader-----------------------------------------------------------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"x64\\Debug\\VertexShaderTest.cso", &pBlob) >> chk;
	pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader) >> chk;

	CHECK_INFOQUEUE(pImmediateContext->VSSetShader(pVertexShader.Get(), NULL, 0u));


	//input layout-----------------------------------------------------------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u});
	ied.push_back({"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });

	pd3dDevice->CreateInputLayout(ied.data(), (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->IASetInputLayout(pInputLayout.Get()) );


	//pixel shader-----------------------------------------------------------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"x64\\Debug\\PixelShaderTest.cso", &pBlob) >> chk;
	pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0u));


	//texture-----------------------------------------------------------------------------------------------------------------------------------------
	int width;
	int height;
	int nrChannels;
	const char* fileName = "Resources\\Textures\\FatihChan.png";

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName, &width, &height, &nrChannels, STBI_rgb_alpha);
	
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture2D;

	D3D11_TEXTURE2D_DESC td = {};
	td.Width = width;
	td.Height = height;
	td.MipLevels = 0;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	td.CPUAccessFlags = 0u;
	td.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

	pd3dDevice->CreateTexture2D(&td, nullptr, &pTexture2D) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->UpdateSubresource(pTexture2D.Get(), 0u, nullptr, data, sizeof(int) * width, 0u) );

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;

	pd3dDevice->CreateShaderResourceView(pTexture2D.Get(), &srvd, &pTextureView) >> chk;
	CHECK_INFOQUEUE( pImmediateContext->GenerateMips(pTextureView.Get()) );

	stbi_image_free(data);

	CHECK_INFOQUEUE( pImmediateContext->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf()) );


	//Sampler State-----------------------------------------------------------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSamplerState;

	D3D11_SAMPLER_DESC sampd = {};
	sampd.Filter = D3D11_FILTER_ANISOTROPIC;
	sampd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampd.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
	sampd.MipLODBias = 0.0f;
	sampd.MinLOD = 0.0f;
	sampd.MaxLOD = D3D11_FLOAT32_MAX;

	pd3dDevice->CreateSamplerState(&sampd, &pSamplerState) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf()) );


	//Draw Frame-----------------------------------------------------------------------------------------------------------------------------------------

	//clear back buffer
	const float color[] = { 0.0f,0.0f,0.0f,0.0f };
	CHECK_INFOQUEUE( pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color) );

	//draw call
	CHECK_INFOQUEUE(pImmediateContext->DrawIndexed(std::size(indices), 0u, 0u));

	//swap buffer
	pSwapChain->Present(1u, 0) >> chk;
}

