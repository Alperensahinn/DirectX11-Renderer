#include "EngineWindow.h"

std::wstring engine::window::GetErrorDescription(HRESULT hr)
{
	wchar_t* descriptionWinalloc = nullptr;
	const auto result = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPWSTR>(&descriptionWinalloc), 0, nullptr
	);

	std::wstring description;
	if (!result) {
		std::cout << "Failed formatting windows error!" << std::endl;
	}
	else {
		description = descriptionWinalloc;
		if (LocalFree(descriptionWinalloc)) {
			std::cout << "Failed freeing memory for windows error formatting" << std::endl;
		}
		if (description.ends_with(L"\r\n")) {
			description.resize(description.size() - 2);
		}
	}

	return description;
}
