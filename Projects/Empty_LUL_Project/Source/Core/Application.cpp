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

    std::shared_ptr<LUL_::EmptyWindow> ew = std::make_shared<LUL_::EmptyWindow>();
    ew->SetDimensions(1920, 1080);
    ew->Show();
    m_pRenderer = std::make_shared<LUL_::DX12::Renderer>(ew);
    m_pRenderer->Initialize();

    LUL_::World::Get().InitializeWorld(m_pRenderer);

    m_dbgEntity = new LUL_::DX12::DebugEntity();
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
