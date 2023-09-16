#pragma once
#include "Direct3D11Renderer.h"
#include "Direct3D11ModelConstantBuffer.h"
#include "..\Material.h"
//#include "..\Mesh.h"

class MeshData;

class Direct3D11Drawable
{
public:
	Direct3D11Drawable(Direct3D11Renderer& pd3dRenderer, std::shared_ptr<MeshData>& mesh, std::shared_ptr<Material>& material);

	void Draw(Direct3D11Renderer& pd3dRenderer);
	virtual DirectX::XMMATRIX GetModelMatrix() = 0;
protected:
	std::shared_ptr<MeshData> pMesh;
	std::shared_ptr<Material> pMaterial;
	std::unique_ptr<Direct3D11ModelConstantBuffer> pConstantBuffer;
};