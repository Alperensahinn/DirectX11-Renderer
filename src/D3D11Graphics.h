#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "DxgiInfoManager.h"

class D3D11Graphics
{
public:
	D3D11Graphics(HWND hWnd);

	void Draw();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device> pd3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
private:
	DxgiInfoManager infoManager;
};
