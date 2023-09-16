#include "App.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

namespace engine::app
{
	App::App()
	{
		pEngineWindow = std::make_unique<engine::window::EngineWindow>();

		light = std::make_unique<Light>(pEngineWindow.get()->GetD3D11Renderer());


		model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\Sponza\\sponza.obj", "Resources\\Sponza\\");
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

			pEngineWindow.get()->GetD3D11Renderer().GetCamera()->Update(&pEngineWindow.get()->GetGLFWWindow(), deltaTime);


			rotateAround += 0.5f;

			if (rotateAround > 360.f) rotateAround = 0.0f;
			
			//draw calls
			pEngineWindow.get()->GetD3D11Renderer().StartFrame();

			light.get()->Bind(pEngineWindow.get()->GetD3D11Renderer());

			model_a.get()->Draw(pEngineWindow.get()->GetD3D11Renderer(), pEngineWindow.get()->GetD3D11Renderer().GetCamera().get()->GetViewMatrix(), pEngineWindow.get()->GetD3D11Renderer().GetCamera().get()->GetProjectionMatrix(), 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			pEngineWindow.get()->GetD3D11Renderer().EndFrame();
		}
	}
}