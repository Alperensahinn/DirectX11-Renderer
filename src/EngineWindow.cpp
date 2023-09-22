#include "EngineWindow.h"

int engine::window::windowWidth = 1024;
int engine::window::windowHeight = 1024;

std::wstring engine::window::GetErrorDescription(HRESULT hr)
{
	wchar_t* descriptionWinalloc = nullptr;
	const auto result = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&descriptionWinalloc), 0, nullptr
	);

	std::wstring description;
	if (!result) {
		std::cout << "Failed formatting windows error!" << std::endl;
	}
	else {
		description = descriptionWinalloc;
		if (LocalFree(descriptionWinalloc)) {
			std::cout << "Failed freeing memory for windows error formatting" << std::endl;
		}
		if (description.ends_with(L"\r\n")) {
			description.resize(description.size() - 2);
		}
	}

	return description;
}

namespace engine::window
{
	EngineWindow::EngineWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		pGLFWWindow = glfwCreateWindow(engine::window::windowWidth, engine::window::windowHeight, "Renderer (Direct3D_11)", NULL, NULL);

		glfwMakeContextCurrent(pGLFWWindow);

		hWnd = glfwGetWin32Window(pGLFWWindow);

		input = std::make_unique<Input>(pGLFWWindow);

		pd3d11Renderer = std::make_unique<Direct3D11Renderer>(hWnd);
	}

	EngineWindow::~EngineWindow()
	{
		glfwTerminate();
	}

	GLFWwindow& EngineWindow::GetGLFWWindow()
	{
		return *pGLFWWindow;
	}

	Direct3D11Renderer& EngineWindow::GetD3D11Renderer()
	{
		return *pd3d11Renderer;
	}
}
