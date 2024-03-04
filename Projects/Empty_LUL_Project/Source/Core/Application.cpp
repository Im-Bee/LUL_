#include "PCH.hpp"

#include "Application.hpp"

#include "Window.hpp"

LUL_DECLARE_APPLICATION(Application);

// Application -----------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void Application::Initialize()
{
    LUL_PROFILER_TIMER_START();

    m_pRenderer = std::make_shared<LUL_::Graphics::DX12::Renderer>(LUL_::Graphics::CreateTarget);
    m_pRenderer->Initialize();
}

// -----------------------------------------------------------------------------
void Application::Update()
{
    m_pRenderer->Update();
    m_pRenderer->Render();
}

// -----------------------------------------------------------------------------
void Application::Destroy()
{
    m_pRenderer->Destroy();

    LUL_::AppProperties::Get().CreatePath(L"C:\\Home\\Desktop\\Results\\");
    LUL_PROFILER_TIMER_SET_OUTPUT_DIR(L"C:\\Home\\Desktop\\Results\\");
}
