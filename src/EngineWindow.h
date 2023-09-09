#pragma once

#include <iostream>
#include <Windows.h>

namespace engine::window
{
	extern int windowWidth;
	extern int windowHeight;

	std::wstring GetErrorDescription(HRESULT hr);
}
