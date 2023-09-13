#include "Direct3D11ModelConstantBuffer.h"
#include "Direct3D11Drawable.h"

Direct3D11ModelConstantBuffer::Direct3D11ModelConstantBuffer(Direct3D11Renderer& pd3dRenderer, Direct3D11Drawable& drawable) : drawable(drawable)
{
	pConstantBuffer = std::make_unique<Direct3D11ConstantBuffer<ModelConstantBuffer>>
	(
		pd3dRenderer, Direct3D11ConstantBuffer<ModelConstantBuffer>::ConstantBufferType::VertexShaderConstantBuffer, 0u
	);
}

void Direct3D11ModelConstantBuffer::Bind(Direct3D11Renderer& pd3dRenderer, DirectX::XMMATRIX viewProjection)
{
	pConstantBuffer.get()->Bind(pd3dRenderer);

	std::unique_ptr<ModelConstantBuffer> cbData = std::make_unique<ModelConstantBuffer>();
	cbData.get()->modelViewPorjection = DirectX::XMMatrixTranspose(drawable.GetModelMatrix() * viewProjection);

	pConstantBuffer.get()->UpdateData(pd3dRenderer, cbData);
}
