#pragma once

#ifdef _WIN32
#include "LUL_.h"

#include "Primitives/EmptyWindow.hpp"

INT CALLBACK WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR cmdline, INT cmdshow)
{
    LUL_::AppProperties::Get();
    LUL_::Logger::Get();

    auto app = LUL_::EntryApplication();
}
#endif // _WIN32
