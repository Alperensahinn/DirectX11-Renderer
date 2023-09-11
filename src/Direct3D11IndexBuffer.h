#pragma once

#include "Direct3D11Renderer.h"
#include <wrl.h>

class Direct3D11IndexBuffer
{
public:
	Direct3D11IndexBuffer(Direct3D11Renderer& d3dRenderer, std::vector<unsigned int>& indices);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
};