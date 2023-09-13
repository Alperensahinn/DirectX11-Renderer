#pragma once

#include "Direct3D11Renderer.h"
#include <wrl.h>
#include <vector>

class Direct3D11IndexBuffer
{
	friend class Mesh;

public:
	Direct3D11IndexBuffer(Direct3D11Renderer& d3dRenderer, std::vector<unsigned int>& indices);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	unsigned int count;
};