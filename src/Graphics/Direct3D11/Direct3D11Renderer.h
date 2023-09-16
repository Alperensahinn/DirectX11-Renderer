#pragma once

#include "DxgiInfoManager.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

class Direct3D11Renderer
{
public:
	Direct3D11Renderer(HWND hWnd);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetImmediateContext();
	void Draw(unsigned int indexCount);
	void StartFrame();
	void EndFrame();


private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthStencilView;
private:
	DxgiInfoManager infoManager;
};
