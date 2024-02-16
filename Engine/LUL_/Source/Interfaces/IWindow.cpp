#include "LUL_.h"

// IWindow ---------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::IWindow::IWindow() noexcept
{
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
            L"%S | I = %d | Ptr = %p", 
            e.what(),
            m_WindowIndex,
            e.GetPtr());
    }
    catch (std::exception e)
    {
        L_LOG(L_ERROR, L"%S", e.what());
    }
}
