#pragma once

#include "Direct3D11Renderer.h"
#include <d3dcompiler.h>
#include <vector>

class Direct3D11InputLayout
{
public:
	Direct3D11InputLayout(Direct3D11Renderer& d3dRenderer, std::vector<D3D11_INPUT_ELEMENT_DESC>& elementDesc, ID3DBlob* blob);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};