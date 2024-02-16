#pragma once

#include "Interfaces/IWindow.hpp"

namespace LUL_
{
    class LUL_EXPORT EmptyWindow
        : public LUL_::IWindow
    {
    public:

        EmptyWindow() = default;

        ~EmptyWindow() = default;



    };
}