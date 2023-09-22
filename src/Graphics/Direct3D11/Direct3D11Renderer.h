#pragma once

#include "DxgiInfoManager.h"
#include "..\Camera.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

class Direct3D11Renderer
{
public:
	Direct3D11Renderer(HWND hWnd);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetImmediateContext();
	void Draw(unsigned int indexCount);


	void ShadowPass(unsigned int indexCount);
	void LambertianPass(unsigned int indexCount);

	void BindLightSpaceMatrix();

	void StartFrame();
	void EndFrame();

	std::unique_ptr<Camera>& GetCamera();

	bool drawMode = 0;
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pShadowDepthView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShadowTextureView;
private:
	DxgiInfoManager infoManager;
	std::unique_ptr<Camera> camera;
};
