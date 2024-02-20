#pragma once

namespace LUL_::Graphics::DX12
{
    class LUL_EXPORT Renderer
        : public LUL_::Graphics::IRenderer
    {
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

    };
}