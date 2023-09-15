#include "App.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

namespace engine::app
{
	App::App()
	{
		pEngineWindow = std::make_unique<engine::window::EngineWindow>();

		camera = std::make_unique<Camera>(0.0f, 0.0f, -5.0f);

		model_a = std::make_unique<Model_A>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\Sponza\\sponza.obj", "Resources\\Sponza\\");

	}

	void App::Run()
	{
		float rotateAround = 0.0f;

		while (!glfwWindowShouldClose(&pEngineWindow.get()->GetGLFWWindow()))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();

			camera.get()->Update(&pEngineWindow.get()->GetGLFWWindow(), deltaTime);

			rotateAround += 0.5f;

			if (rotateAround > 360.f) rotateAround = 0.0f;
			
			//draw calls
			pEngineWindow.get()->GetD3D11Renderer().StartFrame();

			model_a.get()->Draw(pEngineWindow.get()->GetD3D11Renderer(), camera.get()->GetViewProjection(), 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			pEngineWindow.get()->GetD3D11Renderer().EndFrame();
		}
	}
}