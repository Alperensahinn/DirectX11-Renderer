#include "App.h"
#include "Graphics\Material.h"
#include "Graphics\MaterialFactory.h"
#include "Graphics\Mesh.h"
#include "Graphics\BasicCube.h"


float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

namespace engine::app
{
	App::App()
	{
		pEngineWindow = std::make_unique<engine::window::EngineWindow>();

		camera = std::make_unique<Camera>(0.0f, 0.0f, -5.0f);

		//object creation------------------------------------------------------------------------------------------------------
		std::shared_ptr<Material> material1 = MaterialFactory::CreateMaterial(pEngineWindow.get()->GetD3D11Renderer(), "x64\\Debug\\VertexShaderTest.cso", "x64\\Debug\\PixelShaderTest.cso", "Resources\\Textures\\FatihChan.png");

		std::shared_ptr<Mesh> basicCube = std::make_shared<BasicCube>(pEngineWindow.get()->GetD3D11Renderer());

		model1 = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), basicCube, material1);
		model2 = std::make_unique<Model>(pEngineWindow.get()->GetD3D11Renderer(), basicCube, material1);

		model1.get()->SetPosition(2.0f,	0.0f, 2.0f);
		model2.get()->SetPosition(-2.0f, 0.0f, 2.0f);

		//object creation------------------------------------------------------------------------------------------------------
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

			//model1.get()->SetRotation(0.0f, rotateAround, rotateAround);
			//model2.get()->SetRotation(0.0f, -rotateAround, -rotateAround);

			rotateAround += 0.5f;

			if (rotateAround > 360.f) rotateAround = 0.0f;
			
			//draw calls
			pEngineWindow.get()->GetD3D11Renderer().StartFrame();

			model1.get()->Draw(pEngineWindow.get()->GetD3D11Renderer(), camera.get()->GetViewProjection());

			model2.get()->Draw(pEngineWindow.get()->GetD3D11Renderer(), camera.get()->GetViewProjection());

			pEngineWindow.get()->GetD3D11Renderer().EndFrame();
		}
	}
}