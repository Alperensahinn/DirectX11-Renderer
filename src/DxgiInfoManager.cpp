#include "DxgiInfoManager.h"
#include "GraphicsError.h"

#pragma comment(lib, "dxguid.lib")

DxgiInfoManager::DxgiInfoManager()
{
    typedef HRESULT(WINAPI* LPDXGIGETDEBUGINTERFACE)(REFIID, void**);

    HMODULE dxgidebug = LoadLibraryEx(L"dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (dxgidebug)
    {
        auto dxgiGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
            reinterpret_cast<void*>(GetProcAddress(dxgidebug, "DXGIGetDebugInterface")));

        if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(pDxgiInfoQueue.GetAddressOf()))))
        {
            pDxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            pDxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
        }
    }
}

void DxgiInfoManager::Set() noexcept
{
    next = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DxgiInfoManager::GetMessages() const
{
	CheckerToken chk = {};

	std::vector<std::string> messages;
	const auto end = pDxgiInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT hr;
		SIZE_T messageLength;

		pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength) >> chk;

		auto bytes = std::make_unique<byte[]>(messageLength);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		pDxgiInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength) >> chk;
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}

void DxgiInfoManager::PrintMessageQueue(std::vector<std::string>& messages) const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

	std::cout << "[ERROR] InfoQueue: " << std::endl;

	for(int i = 0; i < messages.size(); i++)
	{
		std::cout << messages.at(i) << std::endl;
	}

	SetConsoleTextAttribute(hConsole, 15);
	CloseHandle(hConsole);
}
