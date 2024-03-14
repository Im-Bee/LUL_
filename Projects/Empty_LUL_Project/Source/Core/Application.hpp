#pragma once

#include "EntryPoint/Win32/Entry.hpp"

#include "Primitives/Entity.hpp"

class Application
    : public LUL_::IApplication
{
public:

    Application() = default;

    ~Application() = default;

public:

    void Initialize() override;

    void Update() override;

    void Destroy() override;

private:

    std::shared_ptr<LUL_::DX12::Renderer> m_pRenderer = std::shared_ptr<LUL_::DX12::Renderer>(nullptr);

    LUL_::DX12::DebugEntity* m_dbgEntity = nullptr;
};
