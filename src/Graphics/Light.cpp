#include "Light.h"


Light::Light(Direct3D11Renderer& d3dRenderer)
{
	lightData = std::make_unique<LightData>();

	DirectX::XMFLOAT4 dir = DirectX::XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f);
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
	float near_plane = 1.0f, far_plane = 20.0f;

	DirectX::XMMATRIX lightProjection = DirectX::XMMatrixOrthographicLH(20.0f, 20.0f, near_plane, far_plane);

	DirectX::XMFLOAT4 eyePos = DirectX::XMFLOAT4(-2.0f, 4.0f, -1.0f, 1.0f);
	DirectX::XMFLOAT4 focusPos = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 upDir = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH
	(
		DirectX::XMLoadFloat4(&eyePos),
		DirectX::XMLoadFloat4(&focusPos),
		DirectX::XMLoadFloat4(&upDir)
	);

	DirectX::XMMATRIX lightSpaceMatrix = DirectX::XMMatrixTranspose(lightView * lightProjection);

	typedef struct ShadowCBuff
	{
		DirectX::XMMATRIX lightSpaceMatrix;
	};

	std::unique_ptr<ShadowCBuff> data = std::make_unique<ShadowCBuff>();
	data.get()->lightSpaceMatrix = lightSpaceMatrix;

	std::unique_ptr<Direct3D11ConstantBuffer<ShadowCBuff>> scbuff = std::make_unique<Direct3D11ConstantBuffer<ShadowCBuff>>(d3dRenderer, data, Direct3D11ConstantBuffer<ShadowCBuff>::VertexShaderConstantBuffer, 1u);
	scbuff.get()->Bind(d3dRenderer);
	
	DirectX::XMVECTOR v1 = DirectX::XMLoadFloat4(&eyePos);
	DirectX::XMVECTOR v2 = DirectX::XMLoadFloat4(&focusPos);

	DirectX::XMVECTOR lightDir = DirectX::XMVectorSubtract(v2, v1);

	lightData.get()->Direction = lightDir;

	pLightConstant.get()->Bind(d3dRenderer);
	pLightConstant.get()->UpdateData(d3dRenderer, lightData);
}

void Light::CalculateLightSpaceMatrix()
{
}

