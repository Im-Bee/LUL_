#include "LUL_3D.hpp"

#include "Renderer.hpp"

#include "Primitives/Entity.hpp"

using Microsoft::WRL::ComPtr;

// Renderer --------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Renderer::Initialize()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Initialize DX12::Renderer | %p", this);

    GetTarget()->Show();

    LoadPipeline();
    InitializePipelineState();
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Renderer::Update()
{
    for (auto& e : m_vpEntities)
        e->Update();
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Renderer::Render()
{
    m_pCommands->BeginVerticesProcessing();
    m_pCommands->ProcessVertices();
    m_pCommands->CloseVerticesProcessing();
    m_pSwapChain->WaitForPrevious();
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Renderer::Destroy()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Destroy DX12::Renderer | %p", this);
    // Force destroy on objects
    if (m_pSwapChain.get())
        m_pSwapChain->WaitForPrevious();

    if (m_pMemory.get())
        m_pMemory->~Memory();
    
    if (m_pSwapChain.get())
        m_pSwapChain->~SwapChain();
    
    if (m_pCommands.get())
        m_pCommands->~Commands();
    
    if (m_pHardware.get())
        m_pHardware->~Hardware();

#ifdef _DEBUG
    L_LOG(L_INFO, L"ReportLiveObjects \\/\\/\\/\\/\\/\\/\\/\\/\\/ ");
    Microsoft::WRL::ComPtr<IDXGIDebug1> dxgiDebug = 0;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiDebug.GetAddressOf()))))
    {
        dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_FLAGS(DXGI_DEBUG_RLO_DETAIL | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
    }
    L_LOG(L_INFO, L"ReportLiveObjects ^^^^^^^^^^ ");
#endif // _DEBUG
}

void LUL_::DX12::Renderer::AddEntity(Entity* e)
{
    m_vpEntities.push_back(e);
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Renderer::CreateResourcesForMesh(Mesh* m, uint32_t uMemSize)
{
    m->SetBuffer(m_pMemory->ReserveMemory(uMemSize, MeshBuffer));
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Renderer::LoadPipeline()
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
    LUL_SET_DX_NAME(factory, L"LUL_Factory");

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
        this,
        m_pHardware,
        m_pSwapChain,
        m_pMemory);
    
    m_pSwapChain->Initialize(
        this,
        m_pHardware,
        m_pMemory,
        m_pCommands);
    
    m_pMemory->Initialize(
        this,
        m_pHardware,
        m_pSwapChain,
        m_pCommands);
    
    m_pHardware->EndCreation();
}

void LUL_::DX12::Renderer::InitializePipelineState()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Load assets DX12::Renderer | %p", this);

    m_pMemory->InitializeRootSignature();
    m_pCommands->InitializePipelineState();
    m_pSwapChain->InitializeFence();
    
    m_pSwapChain->WaitForPrevious();


}
