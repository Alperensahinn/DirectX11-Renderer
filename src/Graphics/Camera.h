#pragma once

#include <DirectXMath.h>

class Camera
{
public:
	Camera(float pos_x, float pos_y, float pos_z);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetViewProjection();
private:
	DirectX::XMFLOAT4 cameraPos;
	DirectX::XMFLOAT4 lookAt;
	DirectX::XMFLOAT4 cameraUp;
};