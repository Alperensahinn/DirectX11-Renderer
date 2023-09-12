#include "App.h"
#include "Graphics\Material.h"
#include "Graphics\Mesh.h"
#include "Graphics\BasicCube.h"

namespace engine::app
{
	App::App()
	{
		pEngineWindow = std::make_unique<engine::window::EngineWindow>();

		std::shared_ptr<Direct3D11VertexShader> pVertexShader = std::make_unique<Direct3D11VertexShader>(pEngineWindow.get()->GetD3D11Renderer(), "x64\\Debug\\VertexShaderTest.cso");
		std::shared_ptr<Direct3D11PixelShader> pPixelShader = std::make_unique<Direct3D11PixelShader>(pEngineWindow.get()->GetD3D11Renderer(), "x64\\Debug\\PixelShaderTest.cso");

		std::shared_ptr<Direct3D11Texture2D> pTexture = std::make_shared<Direct3D11Texture2D>(pEngineWindow.get()->GetD3D11Renderer(), "Resources\\Textures\\FatihChan.png", 0u);

		Material::MATERIAL_DESC mdesc = {};
		mdesc.AlbedoTexture = pTexture;
		mdesc.VertexShader = pVertexShader;
		mdesc.PixelShader = pPixelShader;

		std::shared_ptr<Material> pMaterial = std::make_shared<Material>(mdesc);
		std::shared_ptr<Material> pMaterial2 = std::make_shared<Material>(mdesc);

		std::shared_ptr<Mesh> basicCube = std::make_shared<BasicCube>(pEngineWindow.get()->GetD3D11Renderer());

		pD3D11Drawable = std::make_unique<Direct3D11Drawable>(basicCube, pMaterial);
		pD3D11Drawable2 = std::make_unique<Direct3D11Drawable>(basicCube, pMaterial2);
		//object creation------------------------------------------------------------------------------------------------------
	}
	void App::Run()
	{
		float rotateAround = 0.0f;

		while (!glfwWindowShouldClose(&pEngineWindow.get()->GetGLFWWindow()))
		{
			glfwPollEvents();
			
			pD3D11Drawable.get()->Draw(pEngineWindow.get()->GetD3D11Renderer());
			pD3D11Drawable2.get()->Draw(pEngineWindow.get()->GetD3D11Renderer());

			pEngineWindow.get()->GetD3D11Renderer().Draw(rotateAround);

			rotateAround += 1.0f;

			if (rotateAround > 360.f) rotateAround = 0.0f;
		}
	}
}