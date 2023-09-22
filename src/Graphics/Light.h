#pragma once
#include "Direct3D11\Direct3D11ConstantBuffer.h"
#include <DirectXMath.h>
#include <memory>

class Light
{
	typedef struct LightData
	{
		DirectX::XMVECTOR Direction;
		DirectX::XMVECTOR Color;
		DirectX::XMMATRIX lightSpaceMatrix;
	};

public:
	Light(Direct3D11Renderer& d3dRenderer);

	void Bind(Direct3D11Renderer& d3dRenderer);
private:
	void CalculateLightSpaceMatrix();

private:
	std::unique_ptr<LightData> lightData;
	std::unique_ptr<Direct3D11ConstantBuffer<LightData>> pLightConstant;
};