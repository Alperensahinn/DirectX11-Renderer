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
	DxgiInfoManager infoManager;
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
};
