#pragma once

#include <iostream>
#include <Windows.h>

namespace engine::window
{
	std::wstring GetErrorDescription(HRESULT hr);
}
