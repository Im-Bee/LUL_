#pragma once

#include "EntryPoint/Win32/Entry.hpp"
#include "Platform/Win64/DirectX12/Renderer.hpp"

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

    std::shared_ptr<LUL_::Graphics::DX12::Renderer> m_pRenderer = std::shared_ptr<LUL_::Graphics::DX12::Renderer>(nullptr);

};
