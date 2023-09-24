#include "Light.h"


Light::Light(Direct3D11Renderer& d3dRenderer)
{
	lightData = std::make_unique<LightData>();

	CalculateLightData();

	pLightConstantPS = std::make_unique<Direct3D11ConstantBuffer<LightData>>
		(
			d3dRenderer, Direct3D11ConstantBuffer<LightData>::ConstantBufferType::PixelShaderConstantBuffer, 1u
		);

	pLightConstantVS = std::make_unique<Direct3D11ConstantBuffer<LightData>>
		(
			d3dRenderer, Direct3D11ConstantBuffer<LightData>::ConstantBufferType::VertexShaderConstantBuffer, 1u
		);
}

void Light::Bind(Direct3D11Renderer& d3dRenderer)
{
	pLightConstantPS.get()->Bind(d3dRenderer);
	pLightConstantPS.get()->UpdateData(d3dRenderer, lightData);

	pLightConstantVS.get()->Bind(d3dRenderer);
	pLightConstantVS.get()->UpdateData(d3dRenderer, lightData);
}

void Light::CalculateLightData()
{
	float near_plane = 1.0f, far_plane = 50.0f;

	DirectX::XMMATRIX lightProjection = DirectX::XMMatrixOrthographicLH(40.0f, 40.0f, near_plane, far_plane);

	DirectX::XMFLOAT4 eyePos = DirectX::XMFLOAT4(-20.0f, 40.0f, -5.0f, 1.0f);
	DirectX::XMFLOAT4 focusPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 upDir = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat4(&eyePos),
		DirectX::XMLoadFloat4(&focusPos),
		DirectX::XMLoadFloat4(&upDir)
	);

	DirectX::XMMATRIX lightSpaceMatrix = DirectX::XMMatrixTranspose(lightView * lightProjection);

	DirectX::XMVECTOR lightDir = DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&focusPos), DirectX::XMLoadFloat4(&eyePos));

	lightData.get()->Direction = lightDir;
	lightData.get()->lightSpaceMatrix = lightSpaceMatrix;

	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.get()->Color = DirectX::XMLoadFloat4(&color);

}

