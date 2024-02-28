#include "LUL_3D.hpp"

#include "Renderer.hpp"

using Microsoft::WRL::ComPtr;

// Renderer --------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Renderer::Initialize()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Initialize DX12::Renderer | %p", this);

    GetTarget()->Show();

    LoadPipeline();
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Renderer::Update()
{
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Renderer::Render()
{
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Renderer::Destroy()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Destroy DX12::Renderer | %p", this);
    // Force destroy on unused objects

    if (m_pMemory.get())
        m_pMemory->~Memory();

    if (m_pSwapChain.get())
        m_pSwapChain->~SwapChain();

    if (m_pCommands.get())
        m_pCommands->~Commands();

    if (m_pHardware.get())
        m_pHardware->~Hardware();
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Renderer::LoadPipeline()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Load pipeline DX12::Renderer | %p", this);
    
    uint32_t dxgiFactoryFlags = 0;
    #ifdef _DEBUG
    {
        ComPtr<ID3D12Debug> debugController = 0;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
        {
            debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    #endif // _DEBUG

    ComPtr<IDXGIFactory> factory = 0;
    L_THROW_IF_FAILED(CreateDXGIFactory(IID_PPV_ARGS(&factory)));

    L_LOG(L_INFO, L"Create factory | %p", factory.Get());

    m_pHardware = std::make_shared<DX12::Hardware>();
    m_pSwapChain = std::make_shared<DX12::SwapChain>();
    m_pMemory = std::make_shared<DX12::Memory>();
    m_pCommands = std::make_shared<DX12::Commands>();

    m_pHardware->Initialize(
        factory,
        this,
        m_pSwapChain,
        m_pMemory,
        m_pCommands);

    m_pCommands->Initialize(
        factory,
        this,
        m_pHardware,
        m_pSwapChain,
        m_pMemory);

    m_pSwapChain->Initialize(
        factory,
        this,
        m_pHardware,
        m_pMemory,
        m_pCommands);

    m_pMemory->Initialize(
        factory,
        this,
        m_pHardware,
        m_pSwapChain,
        m_pCommands);
}
