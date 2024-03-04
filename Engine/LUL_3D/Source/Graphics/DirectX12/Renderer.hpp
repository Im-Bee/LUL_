#pragma once

#ifdef _WIN64
#include "Hardware.hpp"
#include "SwapChain.hpp"
#include "Memory.hpp"
#include "Commands.hpp"
#include "Vertex.hpp"

namespace LUL_::Graphics::DX12
{
    class LUL_EXPORT Renderer
        : public LUL_::Graphics::IRenderer
    {
    private:

        std::shared_ptr<LUL_::Graphics::DX12::Hardware> m_pHardware = std::shared_ptr<LUL_::Graphics::DX12::Hardware>(nullptr);
        std::shared_ptr<LUL_::Graphics::DX12::SwapChain> m_pSwapChain = std::shared_ptr<LUL_::Graphics::DX12::SwapChain>(nullptr);
        std::shared_ptr<LUL_::Graphics::DX12::Memory> m_pMemory = std::shared_ptr<LUL_::Graphics::DX12::Memory>(nullptr);
        std::shared_ptr<LUL_::Graphics::DX12::Commands> m_pCommands = std::shared_ptr<LUL_::Graphics::DX12::Commands>(nullptr);

    public:

        explicit Renderer(
            RendererFlags flags = 0) noexcept 
            : IRenderer(flags)
        {
        }

        explicit Renderer(
            std::shared_ptr<LUL_::IWindow> target, 
            RendererFlags flags = 0) noexcept
            : IRenderer(target, flags)
        {
        }

        ~Renderer() noexcept = default;

    public:

        virtual void Initialize() override;

        virtual void Update() override;

        virtual void Render() override;

        virtual void Destroy() override;

    private:

        void LoadPipeline();

        void LoadAssets();

    };
}
#endif // _WIN64
