#include "PCH.hpp"

#include "Application.hpp"

#include "Window.hpp"

LUL_DECLARE_APPLICATION(Application);

// Application -----------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Application::Initialize()
{
    Window* w = new Window();
    w->Show();
    w->SetDimensions(500, 100);
}

// -----------------------------------------------------------------------------
void Application::Update()
{
}

// -----------------------------------------------------------------------------
void Application::Destroy()
{
}
