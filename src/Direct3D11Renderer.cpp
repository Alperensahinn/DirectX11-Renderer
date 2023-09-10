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
#include "Direct3D11VertexShader.h"
#include "Direct3D11PixelShader.h"


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

	std::unique_ptr<Direct3D11VertexShader> vertexShader = std::make_unique<Direct3D11VertexShader>(*this, "x64\\Debug\\VertexShaderTest.cso");
	vertexShader.get()->Bind(*this);


	//input layout-----------------------------------------------------------------------------------------------------------------------------------------
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied;
	ied.push_back({"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, 0u, D3D11_INPUT_PER_VERTEX_DATA, 0u});
	ied.push_back({"TexCoord", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0u });

	pd3dDevice->CreateInputLayout(ied.data(), (UINT)std::size(ied), vertexShader.get()->GetBlob()->GetBufferPointer(), vertexShader.get()->GetBlob()->GetBufferSize(), &pInputLayout) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->IASetInputLayout(pInputLayout.Get()) );


	//pixel shader-----------------------------------------------------------------------------------------------------------------------------------------
	std::unique_ptr<Direct3D11PixelShader> pixelShader = std::make_unique<Direct3D11PixelShader>(*this, "x64\\Debug\\PixelShaderTest.cso");
	pixelShader.get()->Bind(*this);
	

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


	//Math test
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	transform = DirectX::XMMatrixMultiply(transform, DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(90.0f)));


	//constant buffer
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;

	struct VSCBUFF
	{
		DirectX::XMMATRIX transform;
	}vcbuffData;
	
	vcbuffData.transform = transform;

	D3D11_BUFFER_DESC cbDesc = {};
	cbDesc.ByteWidth = (sizeof(VSCBUFF));
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA cbdata;
	cbdata.pSysMem = &vcbuffData;
	cbdata.SysMemPitch = 0;
	cbdata.SysMemSlicePitch = 0;

	pd3dDevice->CreateBuffer(&cbDesc, &cbdata, &pConstantBuffer) >> chk;

	CHECK_INFOQUEUE( pImmediateContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf()) );

	D3D11_MAPPED_SUBRESOURCE ms = {};

	pImmediateContext->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &ms) >> chk;
	memcpy(ms.pData, &vcbuffData, sizeof(vcbuffData));
	CHECK_INFOQUEUE( pImmediateContext->Unmap(pConstantBuffer.Get(), 0u) );


	//Draw Frame-----------------------------------------------------------------------------------------------------------------------------------------

	//clear back buffer
	const float color[] = { 0.0f,0.0f,0.0f,0.0f };
	CHECK_INFOQUEUE( pImmediateContext->ClearRenderTargetView(pRenderTargetView.Get(), color) );

	//draw call
	CHECK_INFOQUEUE(pImmediateContext->DrawIndexed(std::size(indices), 0u, 0u));

	//swap buffer
	pSwapChain->Present(1u, 0) >> chk;
}

