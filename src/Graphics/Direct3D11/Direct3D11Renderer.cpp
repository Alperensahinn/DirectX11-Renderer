#include "Direct3D11Renderer.h"
#include "DirectX11Error.h"
#include "..\..\Macros.h"
#include "..\..\EngineWindow.h"

#define STB_IMAGE_IMPLEMENTATION    
#include "..\..\3rdparty\stb_image.h"


#include <iostream>
#include <d3dcompiler.h>

#include "Direct3D11ConstantBuffer.h"

#include "..\Model.h"

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

	//CHECK_INFOQUEUE( pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get()));


	//shadow depth buffer
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pShadowDepthTexture;
	D3D11_TEXTURE2D_DESC shadowTextureDesc = {};
	shadowTextureDesc.Width = engine::window::windowWidth;
	shadowTextureDesc.Height = engine::window::windowHeight;
	shadowTextureDesc.MipLevels = 1u;
	shadowTextureDesc.ArraySize = 1u;
	shadowTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	shadowTextureDesc.SampleDesc.Count = 1u;
	shadowTextureDesc.SampleDesc.Quality = 0u;
	shadowTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	shadowTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	pd3dDevice->CreateTexture2D(&shadowTextureDesc, nullptr, &pShadowDepthTexture) >> chk;

	D3D11_DEPTH_STENCIL_VIEW_DESC shadowDSVdesc = {};
	shadowDSVdesc.Format = DXGI_FORMAT_D32_FLOAT;
	shadowDSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	pd3dDevice->CreateDepthStencilView(pShadowDepthTexture.Get(), &shadowDSVdesc, &pShadowDepthView) >> chk;

	D3D11_SHADER_RESOURCE_VIEW_DESC shadowViewDesc;
	shadowViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shadowViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shadowViewDesc.Texture2D.MostDetailedMip = 0;
	shadowViewDesc.Texture2D.MipLevels = 1;

	pd3dDevice->CreateShaderResourceView(pShadowDepthTexture.Get(), &shadowViewDesc, pShadowTextureView.GetAddressOf()) >> chk;




	//pShadowTexture2D

	CHECK_INFOQUEUE(pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pShadowDepthView.Get()));













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

	//camera test
	camera = std::make_unique<Camera>(0.0f, 5.0f, -5.0f);
}

ID3D11Device* Direct3D11Renderer::GetDevice()
{
	return pd3dDevice.Get();
}

ID3D11DeviceContext* Direct3D11Renderer::GetImmediateContext()
{
	return pImmediateContext.Get();
}

void Direct3D11Renderer::Draw(unsigned int indexCount)
{
	CheckerToken chk = {};

	const float color[] = { 0.0f,0.0f,0.0f,0.0f };

	if(drawMode == 0)
	{
		CHECK_INFOQUEUE(pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color));
		CHECK_INFOQUEUE(pImmediateContext->ClearDepthStencilView(pShadowDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0));

		CHECK_INFOQUEUE(pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pShadowDepthView.Get()));

		RenderShadows();
		CHECK_INFOQUEUE(pImmediateContext->DrawIndexed(indexCount, 0u, 0u));
	}

	else if(drawMode == 1)
	{
		CHECK_INFOQUEUE(pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color));
		CHECK_INFOQUEUE(pImmediateContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0));

		CHECK_INFOQUEUE(pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get()));

		CHECK_INFOQUEUE(pImmediateContext->PSSetShaderResources(0u, 1u, pShadowTextureView.GetAddressOf()))

		CHECK_INFOQUEUE(pImmediateContext->DrawIndexed(indexCount, 0u, 0u));

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSRV = NULL;
		pImmediateContext->PSSetShaderResources(0, 1, pSRV.GetAddressOf());
	}
}

void Direct3D11Renderer::RenderShadows()
{
	//CHECK_INFOQUEUE(pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pShadowDepthView.Get()));

	CheckerToken chk = {};

	std::unique_ptr<Direct3D11VertexShader> pVertexShader = std::make_unique<Direct3D11VertexShader>(*this, "x64\\Debug\\VS_Shadow.cso");
	std::unique_ptr<Direct3D11PixelShader> pPixelShader = std::make_unique<Direct3D11PixelShader>(*this, "x64\\Debug\\PS_Shadow.cso");

	pVertexShader.get()->Bind(*this);
	pPixelShader.get()->Bind(*this);


	float near_plane = 1.0f, far_plane = 20.0f;

	DirectX::XMMATRIX lightProjection = DirectX::XMMatrixOrthographicLH(20.0f, 20.0f, near_plane, far_plane);
	//glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);


	DirectX::XMFLOAT4 eyePos = DirectX::XMFLOAT4(-2.0f, 4.0f, -1.0f, 1.0f);
	DirectX::XMFLOAT4 focusPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 upDir = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat4(&eyePos),
		DirectX::XMLoadFloat4(&focusPos),
		DirectX::XMLoadFloat4(&upDir)
	);

	DirectX::XMMATRIX lightSpaceMatrix = DirectX::XMMatrixTranspose(lightView * lightProjection);

	/*
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	*/

	typedef struct ShadowCBuff
	{
		DirectX::XMMATRIX lightSpaceMatrix;
	};

	std::unique_ptr<ShadowCBuff> data = std::make_unique<ShadowCBuff>();
	data.get()->lightSpaceMatrix = lightSpaceMatrix;

	std::unique_ptr<Direct3D11ConstantBuffer<ShadowCBuff>> scbuff = std::make_unique<Direct3D11ConstantBuffer<ShadowCBuff>>(*this, data, Direct3D11ConstantBuffer<ShadowCBuff>::VertexShaderConstantBuffer, 1u);
	scbuff.get()->Bind(*this);
}


void Direct3D11Renderer::StartFrame()
{
	const float color[] = { 0.0f,0.0f,0.0f,0.0f };
	//CHECK_INFOQUEUE(pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color));
	//CHECK_INFOQUEUE(pImmediateContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0));

	//CHECK_INFOQUEUE(pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color));
	//CHECK_INFOQUEUE(pImmediateContext->ClearDepthStencilView(pShadowDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0));
}

void Direct3D11Renderer::EndFrame()
{
	CheckerToken chk = {};
	pSwapChain->Present(1u, 0) >> chk;
}

std::unique_ptr<Camera>& Direct3D11Renderer::GetCamera()
{
	return camera;
}

