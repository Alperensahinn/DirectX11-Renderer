#pragma once
#include "Direct3D11Renderer.h"
#include "..\Material.h"
#include "..\Mesh.h"

class Direct3D11Drawable
{
public:
	Direct3D11Drawable(std::shared_ptr<Mesh>& mesh, std::shared_ptr<Material>& material);

	void Draw(Direct3D11Renderer& pd3dRenderer);
protected:
	std::shared_ptr<Mesh> pMesh;
	std::shared_ptr<Material> pMaterial;
};