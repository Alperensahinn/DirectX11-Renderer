#include "Direct3D11Drawable.h"

Direct3D11Drawable::Direct3D11Drawable(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material)
{
	this->pMesh = mesh;
	this->pMaterial = material;
}

void Direct3D11Drawable::Draw(Direct3D11Renderer& pd3dRenderer)
{
	pMesh.get()->Bind(pd3dRenderer);
	pMaterial.get()->Bind(pd3dRenderer);
}
