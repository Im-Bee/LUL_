#include "LUL_3D.hpp"

#include "Primitives/EmptyWindow.hpp"

// IRenderer -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::IRenderer::SetTarget(std::shared_ptr<LUL_::IWindow> target) noexcept
{
    L_LOG(L_INFO, L"Rendere setting target to %lS", target->GetWindowName());

    m_pTarget = target;
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
bool LUL_::Graphics::IRenderer::IsFlagSet(RendererFlags f) noexcept
{
    return false != (m_Flags & f);
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::IRenderer::ProcessFlags() noexcept
{
    if (IsFlagSet(Graphics::CreateTarget))
    {
        m_pTarget = std::make_shared<LUL_::EmptyWindow>();

        dynamic_cast<LUL_::EmptyWindow*>(m_pTarget.get())->SetDimensions(1920, 1080);
    }
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::IRenderer::SetFlag(RendererFlags f) noexcept
{
    m_Flags |= f;
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::IRenderer::UnsetFlag(RendererFlags f) noexcept
{
    m_Flags &= ~f;
}
