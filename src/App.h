#pragma once
#include "EngineWindow.h"
#include "Graphics\Direct3D11\Direct3D11Drawable.h"

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

		std::unique_ptr<Direct3D11Drawable> pD3D11Drawable;
		std::unique_ptr<Direct3D11Drawable> pD3D11Drawable2;
	};
}
