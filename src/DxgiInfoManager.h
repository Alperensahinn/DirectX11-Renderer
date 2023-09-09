#pragma once

#include <wrl.h>
#include <dxgidebug.h>
#include <vector>
#include <string>

class DxgiInfoManager
{
public:
	DxgiInfoManager();
	~DxgiInfoManager() = default;
	void Set() noexcept;
	std::vector<std::string> GetMessages() const;
	void PrintMessageQueue(std::vector<std::string>& messages) const;
private:
	unsigned long long next = 0u;
	Microsoft::WRL::ComPtr<IDXGIInfoQueue> pDxgiInfoQueue;
};