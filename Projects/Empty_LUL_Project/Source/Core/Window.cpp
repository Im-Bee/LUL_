#include "PCH.hpp"

#include "Window.hpp"

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(m_HWND, uMsg, wParam, lParam);
}
