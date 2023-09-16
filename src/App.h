#pragma once
#include "EngineWindow.h"
#include "Graphics\Direct3D11\Direct3D11Drawable.h"
#include "Graphics\Light.h"

#include "Graphics\Model.h"

#include <memory>

namespace engine::app
{
	class App
	{
	public:
		App();

		void Run();
	private:
		std::unique_ptr<engine::window::EngineWindow> pEngineWindow;

		std::unique_ptr<Light> light;

		std::unique_ptr<Model> model_a;
	};
}
