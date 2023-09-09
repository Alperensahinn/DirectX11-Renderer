#include "GraphicsError.h"
#include <iostream>

#include < stdlib.h >

HrGrabber::HrGrabber(HRESULT hr, std::source_location loc) noexcept
	:
	hr(hr),
	loc(loc)
{}

void operator>>(HrGrabber grabber, CheckerToken)
{
	if (FAILED(grabber.hr))
	{
		std::cout << "[ERROR] HRESULT failed with error code:\n"
			<< grabber.loc.file_name() << "\n"
			<< grabber.loc.line() << "\n"
			<< grabber.hr << std::endl;
	}
}
