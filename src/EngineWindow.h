#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW\glfw3.h"
#include "GLFW\glfw3native.h"

#include "Input.h"

#include "Graphics\Direct3D11\Direct3D11Renderer.h"

#include <iostream>
#include <Windows.h>

namespace engine::window
{
	class EngineWindow
	{
	public:
		EngineWindow();
		~EngineWindow();

		GLFWwindow& GetGLFWWindow();
		Direct3D11Renderer& GetD3D11Renderer();
	private:
		GLFWwindow* pGLFWWindow;
		HWND hWnd;
		std::unique_ptr<Input> input;
		std::unique_ptr<Direct3D11Renderer> pd3d11Renderer;
	};

	extern int windowWidth;
	extern int windowHeight;

	std::wstring GetErrorDescription(HRESULT hr);
}
