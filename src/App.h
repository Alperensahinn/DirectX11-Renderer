#pragma once
#include "EngineWindow.h"
#include "Graphics\Direct3D11\Direct3D11Drawable.h"
#include "Graphics\Camera.h"

#include "Graphics\Model_A.h"

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

		std::unique_ptr<Camera> camera;

		std::unique_ptr<Model_A> model_a;
	};
}
