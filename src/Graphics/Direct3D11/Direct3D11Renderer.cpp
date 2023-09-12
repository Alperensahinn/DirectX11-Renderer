#include "Direct3D11Renderer.h"
#include "DirectX11Error.h"
#include "..\..\Macros.h"
#include "..\..\EngineWindow.h"

#define STB_IMAGE_IMPLEMENTATION    
#include "..\..\3rdparty\stb_image.h"


#include <iostream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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

	//depth buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthTexture;
	D3D11_TEXTURE2D_DESC dtdesc = {};
	dtdesc.Width = engine::window::windowWidth;
	dtdesc.Height = engine::window::windowHeight;
	dtdesc.MipLevels = 1u;
	dtdesc.ArraySize = 1u;
	dtdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtdesc.SampleDesc.Count = 1u;
	dtdesc.SampleDesc.Quality = 0u;
	dtdesc.Usage = D3D11_USAGE_DEFAULT;
	dtdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pd3dDevice->CreateTexture2D(&dtdesc, NULL, &pDepthTexture) >> chk;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc = {};
	dsvdesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	pd3dDevice->CreateDepthStencilView(pDepthTexture.Get(), &dsvdesc, &pDepthStencilView) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get()));

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

void Direct3D11Renderer::Draw(float rotateAround)
{
	CheckerToken chk = {};

	//index buffer-----------------------------------------------------------------------------------------------------------------------------------------
	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(4);
	indices.push_back(6);
	indices.push_back(4);
	indices.push_back(7);

	indices.push_back(10);
	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(9);
	indices.push_back(11);
	
	indices.push_back(14);
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);
	indices.push_back(13);
	indices.push_back(15);


	//std::unique_ptr<Direct3D11IndexBuffer> indexBuffer = std::make_unique<Direct3D11IndexBuffer>(*this, indices);
	//indexBuffer.get()->Bind(*this);
	

	//vertex shader-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11VertexShader> vertexShader = std::make_unique<Direct3D11VertexShader>(*this, "x64\\Debug\\VertexShaderTest.cso");


	//input layout-----------------------------------------------------------------------------------------------------------------------------------------
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({ "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u });
	ied.push_back({ "TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });

	std::unique_ptr<Direct3D11InputLayout> inputLayout = std::make_unique<Direct3D11InputLayout>(*this, ied, vertexShader.get()->GetBlob());
	inputLayout.get()->Bind(*this);
	

	//Sampler State-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11SamplerState> samplerState = std::make_unique<Direct3D11SamplerState>(*this);
	samplerState.get()->Bind(*this);


	//Math test
	DirectX::XMMATRIX model = DirectX::XMMatrixIdentity();
	//model = DirectX::XMMatrixMultiply(model, DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(90.0f)));
	model = DirectX::XMMatrixMultiply(model, DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(rotateAround)));

	DirectX::XMFLOAT4 cameraPos = DirectX::XMFLOAT4(0.0f, 2.0f, -5.0f, 1.0f);
	DirectX::XMFLOAT4 lookAt = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 cameraUp = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&cameraPos), DirectX::XMLoadFloat4(&lookAt), DirectX::XMLoadFloat4(&cameraUp));
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);

	DirectX::XMMATRIX mvp = DirectX::XMMatrixTranspose(model * view * projection);


	//constant buffer
	struct VSCBUFF
	{
		DirectX::XMMATRIX transform;
	};

	std::unique_ptr<VSCBUFF> cbData = std::make_unique<VSCBUFF>();
	cbData.get()->transform = mvp;

	std::unique_ptr<Direct3D11ConstantBuffer<VSCBUFF>> constantBuffer = std::make_unique<Direct3D11ConstantBuffer<VSCBUFF>>(*this, cbData, Direct3D11ConstantBuffer<VSCBUFF>::ConstantBufferType::VertexShaderConstantBuffer);
	constantBuffer.get()->Bind(*this);
	constantBuffer.get()->UpdateData(*this, cbData);


	//Draw Frame-----------------------------------------------------------------------------------------------------------------------------------------

	//clear back buffer
	const float color[] = { 0.0f,0.0f,0.0f,0.0f };
	CHECK_INFOQUEUE( pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color) );
	CHECK_INFOQUEUE( pImmediateContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0) );

	//draw call
	CHECK_INFOQUEUE(pImmediateContext->DrawIndexed(indices.size(), 0u, 0u));

	//swap buffer
	pSwapChain->Present(1u, 0) >> chk;
}

