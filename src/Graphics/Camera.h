#pragma once

#include <DirectXMath.h>
#include "..\..\Libraries\include\GLFW\glfw3.h"

class EngineWindow;

class Camera
{
public:
	Camera(float pos_x, float pos_y, float pos_z);

	void Update(GLFWwindow* window, float deltaTime);

	DirectX::XMMATRIX GetViewMatrix();
	DirectX::XMMATRIX GetViewProjection();
private:
	void MoveCamera(GLFWwindow* window, float deltaTime);
	void RotateCamera();
private:
	DirectX::XMVECTOR cameraPos;
	DirectX::XMVECTOR cameraFront;
	DirectX::XMVECTOR cameraUp;

	const float cameraSpeed;
	float last_pos_x = 0.0f;
	float last_pos_y = 0.0f;
	float yaw = 90.0f;
	float pitch = 0.0f;
	bool firstMouse;
};