#pragma once

namespace LUL_::Graphics
{
    typedef int RendererFlags;

    static constexpr RendererFlags CreateTarget = 0x01;

    class LUL_EXPORT IRenderer
    {
    private:

        std::shared_ptr<LUL_::IWindow> m_pTarget = std::shared_ptr<LUL_::IWindow>(nullptr);

    public:

        explicit IRenderer(RendererFlags flags = 0) noexcept
        {
            SetFlag(flags);
            ProcessFlags();
        }

        explicit IRenderer(
            std::shared_ptr<LUL_::IWindow> target, 
            RendererFlags flags = 0) noexcept
        {
            SetFlag(flags);
            ProcessFlags();
            SetTarget(target);
        }

        ~IRenderer() noexcept = default;

    public:

        virtual void Initialize() = 0;

        virtual void Update() = 0;

        virtual void Render() = 0;

        virtual void Destroy() = 0;

    public:

        // Setters ---------------------------------------------------------------------

        void SetTarget(std::shared_ptr<LUL_::IWindow> target) noexcept;

    public:

        // Getters ---------------------------------------------------------------------

        const std::shared_ptr<LUL_::IWindow>& GetTarget() const noexcept { return m_pTarget; }

    private:

        void ProcessFlags() noexcept;

        bool IsFlagSet(RendererFlags f) noexcept;

        void SetFlag(RendererFlags f) noexcept;

        void UnsetFlag(RendererFlags f) noexcept;

    private:

        int m_Flags = 0x00;

    };
}
