#pragma once

#include "Direct3D11ConstantBuffer.h"

class Direct3D11Drawable;

class Direct3D11ModelConstantBuffer
{
	struct ModelConstantBuffer
	{
		DirectX::XMMATRIX modelViewPorjection;
		DirectX::XMMATRIX model;
		DirectX::XMMATRIX viewPosition;
	};

public:
	Direct3D11ModelConstantBuffer(Direct3D11Renderer& pd3dRenderer, Direct3D11Drawable& drawable);

	void Bind(Direct3D11Renderer& pd3dRenderer, DirectX::XMMATRIX view, DirectX::XMMATRIX projection);
private:
	std::unique_ptr<Direct3D11ConstantBuffer<ModelConstantBuffer>> pConstantBuffer;
	Direct3D11Drawable& drawable;
};