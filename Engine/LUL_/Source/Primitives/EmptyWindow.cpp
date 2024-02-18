#include "LUL_.h"

#include "EmptyWindow.hpp"

#ifdef _WIN32
// EmptyWindow -----------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::EmptyWindow::EmptyWindow()
    : LUL_::IWindow(L"Unknown", L"Unknown")
{
}

// -----------------------------------------------------------------------------
LUL_::EmptyWindow::EmptyWindow(wchar_t const* const windowName, 
    wchar_t const* const windowClass)
    : LUL_::IWindow(windowName, windowClass)
{
}

// -----------------------------------------------------------------------------
void LUL_::EmptyWindow::Show()
{
    L_LOG(L_INFO, L"Showind window | %lS", GetWindowName());

    if (!Create())
    {
        L_LOG(L_ERROR, L"Couldn't create a window | %lS", GetWindowName());
        DWORD e = GetLastError();
        L_LOG(L_ERROR, L"Error: %u", e);
        return;
    }

    ShowWindow(m_HWND, SW_NORMAL);
}

// -----------------------------------------------------------------------------
void LUL_::EmptyWindow::Close()
{
    DestroyWindow(m_HWND);
}

// -----------------------------------------------------------------------------
void LUL_::EmptyWindow::SetDimensions(L_Vec2<uint32_t> d)
{
    SetWindowDimensions(d);
    SetWindowPos(m_HWND,
        nullptr,
        0,
        0,
        GetWindowDimensions().x,
        GetWindowDimensions().y,
        SWP_NOMOVE | SWP_NOZORDER);
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
LRESULT LUL_::EmptyWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    EmptyWindow* pThis = nullptr;

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
    if (pThis)
    {
        if (uMsg == WM_DESTROY)
            return pThis->OnDestroyMessage();

        if (uMsg == WM_SIZE)
            return pThis->OnResizeMessage(lParam);

        return pThis->HandleMessage(uMsg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

// -----------------------------------------------------------------------------
bool LUL_::EmptyWindow::Create()
{
    WNDCLASSEX wcex = { 0 };

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = EmptyWindow::WindowProc;
    wcex.hInstance = HInstance();
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = GetWindowName();

    RegisterClassEx(&wcex);

    m_HWND = CreateWindow(GetWindowClass(),
        GetWindowName(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        GetWindowDimensions().x,
        GetWindowDimensions().y,
        nullptr,
        nullptr,
        HInstance(),
        this);

    return (m_HWND ? true : false);
}

// -----------------------------------------------------------------------------
LRESULT LUL_::EmptyWindow::OnResizeMessage(LPARAM lParam)
{
    SetWindowDimensions({ LOWORD(lParam), HIWORD(lParam) });

    return 0;
}

// -----------------------------------------------------------------------------
LRESULT LUL_::EmptyWindow::OnDestroyMessage()
{
    L_LOG(L_INFO, L"Closing window | %lS", GetWindowName());

    if (GetWindowIndex() == LUL_MAIN_WINDOW_INDEX)
        LUL_::AppProperties::Get().TryQuitApplication();

    return 0;
}
#endif // _WIN32