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

    public:

#ifdef _WIN32
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            EmptyWindow* pThis = NULL;

            if (uMsg == WM_NCCREATE)
            {
                CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
                pThis = (EmptyWindow*)pCreate->lpCreateParams;
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

                pThis->m_HWND = hwnd;
            }
            else
            {
                pThis = (EmptyWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
            }
            // if (pThis)
            // {
            //     return pThis->HandleMessage(uMsg, wParam, lParam);
            // }
            // else
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }
#endif // _WIN32

    protected:

#ifdef _WIN32
        HWND m_HWND = nullptr;
#endif // _WIN32

    };
}