#include "App.h"

using namespace engine::app;

int main()
{
    try
    {
        App{}.Run();
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, L"Exception", L"Standart Exception", MB_OK | MB_ICONEXCLAMATION);
    }
}