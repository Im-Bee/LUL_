#pragma once

#include "Primitives/EmptyWindow.hpp"

class Window
    : public LUL_::EmptyWindow
{

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

};