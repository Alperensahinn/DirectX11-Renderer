#include "App.h"

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

namespace engine::app
{
	App::App()
	{
		pEngineWindow = std::make_unique<engine::window::EngineWindow>();

		light = std::make_unique<Light>(pEngineWindow.get()->GetD3D11Renderer());

		//model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\Sponza\\sponza.obj", "Resources\\Sponza\\");
		//model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\3DModels\\Cube.obj", "Resources\\3DModels\\");
		model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\3DModels\\ShadowTestObject.obj", "Resources\\3DModels\\");
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


			pEngineWindow.get()->GetD3D11Renderer().drawMode = 0;
			//draw calls
			pEngineWindow.get()->GetD3D11Renderer().StartFrame();

			light.get()->Bind(pEngineWindow.get()->GetD3D11Renderer());

			model_a.get()->Draw(pEngineWindow.get()->GetD3D11Renderer(), 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			pEngineWindow.get()->GetD3D11Renderer().EndFrame();

			pEngineWindow.get()->GetD3D11Renderer().drawMode = 1;
			//draw calls
			pEngineWindow.get()->GetD3D11Renderer().StartFrame();

			light.get()->Bind(pEngineWindow.get()->GetD3D11Renderer());

			model_a.get()->Draw(pEngineWindow.get()->GetD3D11Renderer(), 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

			pEngineWindow.get()->GetD3D11Renderer().EndFrame();
		}
	}
}