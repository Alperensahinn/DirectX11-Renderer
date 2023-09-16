#include "Direct3D11Drawable.h"
#include "..\Model.h"

Direct3D11Drawable::Direct3D11Drawable(Direct3D11Renderer& pd3dRenderer, std::shared_ptr<MeshData>& mesh, std::shared_ptr<Material>& material)
{
	this->pMesh = mesh;
	this->pMaterial = material;
	pConstantBuffer = std::make_unique<Direct3D11ModelConstantBuffer>(pd3dRenderer, *this);
}

void Direct3D11Drawable::Draw(Direct3D11Renderer& pd3dRenderer, DirectX::XMMATRIX cameraView, DirectX::XMMATRIX cameraProjection)
{
	pMesh.get()->Bind(pd3dRenderer);
	pMaterial.get()->Bind(pd3dRenderer);
	pConstantBuffer.get()->Bind(pd3dRenderer, cameraView, cameraProjection);

	pd3dRenderer.Draw(pMesh.get()->GetIndexCount());
}
