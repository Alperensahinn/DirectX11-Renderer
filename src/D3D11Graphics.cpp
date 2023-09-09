#include "D3D11Graphics.h"
#include "GraphicsError.h"
#include "Macros.h"

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

	//vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"x64\\Debug\\VertexShaderTest.cso", &pBlob) >> chk;
	pd3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertexShader) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->VSSetShader(pVertexShader.Get(), NULL, 0u) );


	//vertexbuffer
	const float vertices[] =
	{
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

	unsigned int stride = sizeof(float) * 3;
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

	//input layout
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u});

	pd3dDevice->CreateInputLayout(ied.data(), (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->IASetInputLayout(pInputLayout.Get()) );

	//pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"x64\\Debug\\PixelShaderTest.cso", &pBlob) >> chk;
	pd3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixelShader) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->PSSetShader(pPixelShader.Get(), NULL, 0u));


	//Draw Frame

	//clear back buffer
	const float color[] = { 0.0f,0.0f,0.0f,0.0f };
	CHECK_INFOQUEUE( pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color) );

	//draw call
	CHECK_INFOQUEUE(pImmediateContext->Draw(3u, 0u));

	//swap buffer
	pSwapChain->Present(1u, 0) >> chk;
}

