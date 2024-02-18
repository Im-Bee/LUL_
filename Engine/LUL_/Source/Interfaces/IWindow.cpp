#include "LUL_.h"

// IWindow ---------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::IWindow::IWindow(wchar_t const* const windowName,
    wchar_t const* const windowClass) noexcept
{
    wcscpy_s(m_WindowName, windowName);
    wcscpy_s(m_WindowClass, windowClass);

    AddItselfToAppProperties();
}

// -----------------------------------------------------------------------------
LUL_::IWindow::~IWindow() noexcept
{
    RemoveItselfFromAppProperties();
}

// IWindow ---------------------------------------------------------------------
// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::IWindow::AddItselfToAppProperties() noexcept
{
    m_WindowIndex = LUL_::AppProperties::Get().AddIWindow(this);
}

// -----------------------------------------------------------------------------
void LUL_::IWindow::RemoveItselfFromAppProperties() noexcept
{
    try
    {
        LUL_::AppProperties::Get().RemoveIWindow(this);
    }
    catch (Exceptions::ItemNotFound e)
    {
        L_LOG(L_ERROR, 
            L"%S | I = %d | Ptr = 0x%p", 
            e.what(),
            m_WindowIndex,
            e.GetPtr());
    }
    catch (std::exception e)
    {
        L_LOG(L_ERROR, L"%S", e.what());
    }
}
