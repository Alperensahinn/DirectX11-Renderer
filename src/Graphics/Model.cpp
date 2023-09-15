#include "Model.h"

Model::Model(Direct3D11Renderer& pd3dRenderer, std::shared_ptr<MeshData>& mesh, std::shared_ptr<Material>& material) : Direct3D11Drawable(pd3dRenderer, mesh, material)
{
	pos_x = 0.0f;
	pos_y = 0.0f;
	pos_z = 0.0f;

	rot_x = 0.0f;
	rot_y = 0.0f;
	rot_z = 0.0f;

	scl_x = 1.0f;
	scl_y = 1.0f;
	scl_z = 1.0f;
}

DirectX::XMMATRIX Model::GetModelMatrix()
{
	DirectX::XMMATRIX modelMatrix = DirectX::XMMatrixIdentity();
					
	modelMatrix = DirectX::XMMatrixMultiply(modelMatrix, DirectX::XMMatrixScaling(scl_x, scl_y, scl_z));

	modelMatrix = DirectX::XMMatrixMultiply
	(
		modelMatrix, DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(rot_x), DirectX::XMConvertToRadians(rot_y), DirectX::XMConvertToRadians(rot_z))
	);

	modelMatrix = DirectX::XMMatrixMultiply(modelMatrix, DirectX::XMMatrixTranslation(pos_x, pos_y, pos_z));

	return modelMatrix;
}

void Model::SetPosition(float x, float y, float z)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
}

void Model::SetRotation(float x, float y, float z)
{
	rot_x = x;
	rot_y = y;
	rot_z = z;
}

void Model::SetScale(float x, float y, float z)
{
	scl_x = x;
	scl_y = y;
	scl_z = z;
}
