#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3.h"
#include "GLFW\glfw3native.h"


#include "D3D11Renderer.h"
#include "EngineWindow.h"

#include <iostream>

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(engine::window::windowWidth, engine::window::windowHeight, "Renderer (Direct3D_11)", NULL, NULL);

	if(window == nullptr)
	{
		std::cout << "GLFW Window Creation Failed!";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	HWND hWnd = glfwGetWin32Window(window);

	std::unique_ptr<D3D11Renderer> pGfx = std::make_unique<D3D11Renderer>(hWnd);

	while (!glfwWindowShouldClose(window))
	{
		pGfx->Draw();
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}