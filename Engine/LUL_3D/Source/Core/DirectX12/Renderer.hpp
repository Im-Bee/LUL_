#pragma once

#ifdef _WIN64
#include "Vertex.hpp"
#include "Hardware.hpp"
#include "SwapChain.hpp"
#include "Memory.hpp"
#include "Commands.hpp"

namespace LUL_::DX12
{
    class Mesh;
    class Entity;

    class LUL_EXPORT Renderer
        : public LUL_::IRenderer
    {
    private:
        std::shared_ptr<LUL_::DX12::Hardware> m_pHardware = std::shared_ptr<LUL_::DX12::Hardware>(nullptr);
        std::shared_ptr<LUL_::DX12::SwapChain> m_pSwapChain = std::shared_ptr<LUL_::DX12::SwapChain>(nullptr);
        std::shared_ptr<LUL_::DX12::Memory> m_pMemory = std::shared_ptr<LUL_::DX12::Memory>(nullptr);
        std::shared_ptr<LUL_::DX12::Commands> m_pCommands = std::shared_ptr<LUL_::DX12::Commands>(nullptr);

        std::vector<Entity*> m_vpEntities;

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

    public:
        void CreateResourcesForMesh(Mesh* m, uint32_t uMemSize);

        void AddEntity(Entity* e);

    private:
        void LoadPipeline();
        void InitializePipelineState();
    };
}
#endif // _WIN64
