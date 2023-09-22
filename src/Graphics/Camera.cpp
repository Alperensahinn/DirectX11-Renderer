#include "Camera.h"
#include "..\Input.h"

#include <iostream>

Camera::Camera(float pos_x, float pos_y, float pos_z) : cameraSpeed(0.05), firstMouse(true)
{
	DirectX::XMFLOAT4 tmp_pos = DirectX::XMFLOAT4(pos_x, pos_y, pos_z, 1.0f);
	DirectX::XMFLOAT4 tmp_front = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 tmp_up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	cameraPos = DirectX::XMLoadFloat4(&tmp_pos);
	cameraFront = DirectX::XMLoadFloat4(&tmp_front);
	cameraUp = DirectX::XMLoadFloat4(&tmp_up);
}

void Camera::Update(GLFWwindow* window, float deltaTime)
{
	MoveCamera(window, deltaTime);

	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		RotateCamera();
	}

	else if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
	}
}

DirectX::XMVECTOR Camera::GetPosition()
{
	return cameraPos;
}

DirectX::XMMATRIX Camera::GetViewMatrix()
{
	return DirectX::XMMatrixLookAtLH(cameraPos, DirectX::XMVectorAdd(cameraPos, cameraFront), cameraUp);
}

DirectX::XMMATRIX Camera::GetProjectionMatrix()
{
	return DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 1.0f / 1.0f, 0.1f, 100.0f);
}

void Camera::MoveCamera(GLFWwindow* window, float deltaTime)
{
	using namespace DirectX;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos += DirectX::XMVector4Normalize(DirectX::XMVector3Cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos -= DirectX::XMVector4Normalize(DirectX::XMVector3Cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraUp;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraUp;
	}
}

void Camera::RotateCamera()
{
	if (firstMouse == true)
	{
		last_pos_x = Input::mousePosition.x;
		last_pos_y = Input::mousePosition.y;
		firstMouse = false;
	}

	float xoffset = last_pos_x - Input::mousePosition.x;
	float yoffset = last_pos_y - Input::mousePosition.y;
	last_pos_x = Input::mousePosition.x;
	last_pos_y = Input::mousePosition.y;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	DirectX::XMFLOAT4 front;
	front.w = 0.0f;
	front.x = cos(DirectX::XMConvertToRadians(yaw)) * cos(DirectX::XMConvertToRadians(pitch));
	front.y = sin(DirectX::XMConvertToRadians(pitch));
	front.z = sin(DirectX::XMConvertToRadians(yaw)) * cos(DirectX::XMConvertToRadians(pitch));
	cameraFront = DirectX::XMVector4Normalize(DirectX::XMLoadFloat4(&front));
}
