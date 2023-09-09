#pragma once
#include "EngineWindow.h"
#include <source_location>

struct CheckerToken {};

struct  HrGrabber
{
	HrGrabber(HRESULT hr, std::source_location = std::source_location::current()) noexcept;
	HRESULT hr;
	std::source_location loc;
};

void operator>>(HrGrabber, CheckerToken);

