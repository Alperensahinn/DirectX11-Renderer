#include "Direct3D11Renderer.h"
#include "GraphicsError.h"
#include "Macros.h"
#include "EngineWindow.h"

#define STB_IMAGE_IMPLEMENTATION    
#include "3rdparty\stb_image.h"


#include <iostream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Mesh.h"
#include "Direct3D11VertexBuffer.h"
#include "Direct3D11IndexBuffer.h"
#include "Direct3D11VertexShader.h"
#include "Direct3D11PixelShader.h"
#include "Direct3D11SamplerState.h"
#include "Direct3D11ConstantBuffer.h"
#include "Direct3D11InputLayout.h"
#include "Direct3D11Texture2D.h"



#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")


Direct3D11Renderer::Direct3D11Renderer(HWND hWnd)
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
	vp.Width = engine::window::windowWidth;
	vp.Height = engine::window::windowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	CHECK_INFOQUEUE( pImmediateContext->RSSetViewports(1, &vp) );

	CHECK_INFOQUEUE( pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );
}

ID3D11Device* Direct3D11Renderer::GetDevice()
{
	return pd3dDevice.Get();
}

ID3D11DeviceContext* Direct3D11Renderer::GetImmediateContext()
{
	return pImmediateContext.Get();
}

void Direct3D11Renderer::Draw()
{
	CheckerToken chk = {};


	//vertex buffer-----------------------------------------------------------------------------------------------------------------------------------------
	std::vector<Vertex> vertices;
	vertices.push_back({ 0.5f, 0.5f, 0.0f,	1.0f, 1.0f });
	vertices.push_back({ 0.5f, -0.5f, 0.0f,	1.0f, 0.0f });
	vertices.push_back({ -0.5f, -0.5f, 0.0f,	0.0f, 0.0f });
	vertices.push_back({ -0.5f, 0.5f, 0.0f,	0.0f, 1.0f });

	std::unique_ptr<Direct3D11VertexBuffer<Vertex>> vertexBuffer = std::make_unique<Direct3D11VertexBuffer<Vertex>>(*this, vertices);
	vertexBuffer.get()->Bind(*this);


	//index buffer-----------------------------------------------------------------------------------------------------------------------------------------
	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);


	std::unique_ptr<Direct3D11IndexBuffer> indexBuffer = std::make_unique<Direct3D11IndexBuffer>(*this, indices);
	indexBuffer.get()->Bind(*this);


	//vertex shader-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11VertexShader> vertexShader = std::make_unique<Direct3D11VertexShader>(*this, "x64\\Debug\\VertexShaderTest.cso");
	vertexShader.get()->Bind(*this);


	//input layout-----------------------------------------------------------------------------------------------------------------------------------------
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });

	std::unique_ptr<Direct3D11InputLayout> inputLayout = std::make_unique<Direct3D11InputLayout>(*this, ied, vertexShader.get()->GetBlob());
	inputLayout.get()->Bind(*this);


	//pixel shader-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11PixelShader> pixelShader = std::make_unique<Direct3D11PixelShader>(*this, "x64\\Debug\\PixelShaderTest.cso");
	pixelShader.get()->Bind(*this);
	

	//texture-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11Texture2D> texture2D = std::make_unique<Direct3D11Texture2D>(*this, "Resources\\Textures\\FatihChan.png", 0u);
	texture2D.get()->Bind(*this);


	//Sampler State-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11SamplerState> samplerState = std::make_unique<Direct3D11SamplerState>(*this);
	samplerState.get()->Bind(*this);


	//Math test
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	transform = DirectX::XMMatrixMultiply(transform, DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(90.0f)));


	//constant buffer
	struct VSCBUFF
	{
		DirectX::XMMATRIX transform;
	};

	std::unique_ptr<VSCBUFF> cbData = std::make_unique<VSCBUFF>();
	cbData.get()->transform = transform;

	std::unique_ptr<Direct3D11ConstantBuffer<VSCBUFF>> constantBuffer = std::make_unique<Direct3D11ConstantBuffer<VSCBUFF>>(*this, cbData, Direct3D11ConstantBuffer<VSCBUFF>::ConstantBufferType::VertexShaderConstantBuffer);
	constantBuffer.get()->Bind(*this);
	constantBuffer.get()->UpdateData(*this, cbData);


	//Draw Frame-----------------------------------------------------------------------------------------------------------------------------------------

	//clear back buffer
	const float color[] = { 0.0f,0.0f,0.0f,0.0f };
	CHECK_INFOQUEUE( pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color) );

	//draw call
	CHECK_INFOQUEUE(pImmediateContext->DrawIndexed(indices.size(), 0u, 0u));

	//swap buffer
	pSwapChain->Present(1u, 0) >> chk;
}

