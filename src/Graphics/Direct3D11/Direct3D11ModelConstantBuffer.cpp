#include "Direct3D11ModelConstantBuffer.h"
#include "Direct3D11Drawable.h"

Direct3D11ModelConstantBuffer::Direct3D11ModelConstantBuffer(Direct3D11Renderer& pd3dRenderer, Direct3D11Drawable& drawable) : drawable(drawable)
{
	pConstantBuffer = std::make_unique<Direct3D11ConstantBuffer<ModelConstantBuffer>>
	(
		pd3dRenderer, Direct3D11ConstantBuffer<ModelConstantBuffer>::ConstantBufferType::VertexShaderConstantBuffer, 0u
	);
}

void Direct3D11ModelConstantBuffer::Bind(Direct3D11Renderer& pd3dRenderer)
{
	pConstantBuffer.get()->Bind(pd3dRenderer);

	std::unique_ptr<ModelConstantBuffer> cbData = std::make_unique<ModelConstantBuffer>();
	cbData.get()->modelViewPorjection = DirectX::XMMatrixTranspose(drawable.GetModelMatrix() * (pd3dRenderer.GetCamera().get()->GetViewMatrix() * pd3dRenderer.GetCamera().get()->GetProjectionMatrix()));
	cbData.get()->model = DirectX::XMMatrixTranspose(drawable.GetModelMatrix());
	cbData.get()->viewPosition = pd3dRenderer.GetCamera().get()->GetPosition();

	pConstantBuffer.get()->UpdateData(pd3dRenderer, cbData);
}
