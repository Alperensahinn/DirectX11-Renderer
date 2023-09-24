#pragma once

#include "DxgiInfoManager.h"
#include "..\Camera.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <memory>

class Direct3D11ResourceMap;
class ShadowMapPass;
class ForwardPass;

class Direct3D11Renderer
{
public:
	Direct3D11Renderer(HWND hWnd);
	~Direct3D11Renderer();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetImmediateContext();
	void Draw(unsigned int indexCount);

	void ShadowPass();
	void LambertianPass();

	void EndFrame();

	Direct3D11ResourceMap& GetResourceMap();
	ShadowMapPass& GetShadowMapPass();
	ForwardPass& GetForwardPass();

	std::unique_ptr<Camera>& GetCamera();

	int drawMode = 0; // 0 = shadow pass, 1 = lambertian pass
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pShadowDepthView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShadowTextureView;

	Direct3D11ResourceMap* pResourceMap;
	ShadowMapPass* pShadowMapPass;
	ForwardPass* pForwardPass;
private:
	DxgiInfoManager infoManager;
	std::unique_ptr<Camera> camera;
};
