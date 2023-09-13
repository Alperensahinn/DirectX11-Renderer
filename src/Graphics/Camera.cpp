#include "Camera.h"

Camera::Camera(float pos_x, float pos_y, float pos_z)
{
	cameraPos = DirectX::XMFLOAT4(pos_x, pos_y, pos_z, 1.0f);
	lookAt = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cameraUp = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&cameraPos), DirectX::XMLoadFloat4(&lookAt), DirectX::XMLoadFloat4(&cameraUp));
}

DirectX::XMMATRIX Camera::GetViewProjection()
{
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat4(&cameraPos), DirectX::XMLoadFloat4(&lookAt), DirectX::XMLoadFloat4(&cameraUp));
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	
	return view * projection;
}
