#include "App.h"
#include "Graphics\ShadowMapPass.h"
#include "Graphics\ForwardPass.h"

#include <sstream>

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float counter = 0.0f;
int nubmerOfFrames = 0;

namespace engine::app
{
	App::App()
	{
		pEngineWindow = std::make_unique<engine::window::EngineWindow>();

		light = std::make_unique<Light>(pEngineWindow.get()->GetD3D11Renderer());

		//model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\Sponza\\sponza.obj", "Resources\\Sponza\\");
		model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\BrickCube\\BrickCube.obj", "Resources\\BrickCube\\");
		//model_a = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\3DModels\\Cube.obj", "Resources\\3DModels\\");
		//model_a = std::make_shared<Model>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\3DModels\\ShadowTestObject.obj", "Resources\\3DModels\\");
	}

	void App::Run()
	{
		while (!glfwWindowShouldClose(&pEngineWindow.get()->GetGLFWWindow()))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();

			pEngineWindow.get()->GetD3D11Renderer().GetCamera()->Update(&pEngineWindow.get()->GetGLFWWindow(), deltaTime);

			light.get()->Bind(pEngineWindow.get()->GetD3D11Renderer());

			//shadow pass
			pEngineWindow.get()->GetD3D11Renderer().GetShadowMapPass().AddToPass(model_a);
			pEngineWindow.get()->GetD3D11Renderer().GetShadowMapPass().Execute(pEngineWindow.get()->GetD3D11Renderer());

			//forward pass
			pEngineWindow.get()->GetD3D11Renderer().GetForwardPass().AddToPass(model_a);
			pEngineWindow.get()->GetD3D11Renderer().GetForwardPass().Execute(pEngineWindow.get()->GetD3D11Renderer());

			pEngineWindow.get()->GetD3D11Renderer().EndFrame();

			CalculateFPS();
		}
	}

	void App::CalculateFPS()
	{
		counter = counter + deltaTime;

		nubmerOfFrames++;

		if(counter >= 1.0f)
		{
			int fps = nubmerOfFrames;

			std::stringstream ss;
			ss << "Renderer (Direct3D_11)" << " FPS: " << fps << "(" << deltaTime * 1000.0f << "ms)";

			glfwSetWindowTitle(&pEngineWindow.get()->GetGLFWWindow(), ss.str().c_str());

			nubmerOfFrames = 0;
			counter = 0;
		}
	}
}