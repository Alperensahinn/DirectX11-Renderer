#include "Light.h"


Light::Light(Direct3D11Renderer& d3dRenderer)
{
	lightData = std::make_unique<LightData>();

	DirectX::XMFLOAT4 dir = DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	lightData.get()->Direction = DirectX::XMLoadFloat4(&dir);
	lightData.get()->Color = DirectX::XMLoadFloat4(&color);

	pLightConstant = std::make_unique<Direct3D11ConstantBuffer<LightData>>
		(
			d3dRenderer, Direct3D11ConstantBuffer<LightData>::ConstantBufferType::PixelShaderConstantBuffer, 0u
		);
}

void Light::Bind(Direct3D11Renderer& d3dRenderer)
{
	pLightConstant.get()->Bind(d3dRenderer);
	pLightConstant.get()->UpdateData(d3dRenderer, lightData);
}

