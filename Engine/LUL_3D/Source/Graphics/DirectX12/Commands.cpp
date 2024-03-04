#include "LUL_3D.hpp"

#include "Commands.hpp"

using Microsoft::WRL::ComPtr;

// Commands -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::IUnknown> hardware,
	std::shared_ptr<const LUL_::IUnknown> swapchain,
	std::shared_ptr<const LUL_::IUnknown> memory)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::Commands | %p", this);

	m_pRenderer = renderer;
	if (hardware->GetClass() != DX12::Hardware::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pHardware = hardware;
	if (swapchain->GetClass() != DX12::SwapChain::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pSwapChain = swapchain;
	if (memory->GetClass() != DX12::Memory::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pMemory = memory;

	m_pCommandQueue = LUL_GET_HARDWARE(m_pHardware)->CreateCommandQueue();
	m_pCommandAllocator = LUL_GET_HARDWARE(m_pHardware)->CreateCommandAllocator();
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::InitializeAssets()
{
    LUL_PROFILER_TIMER_START();
    L_LOG(L_INFO, L"Initialize assets LUL_::Graphics::DX12::Commands | %p", this);

    m_pPipelineState = LUL_GET_HARDWARE(m_pHardware)->CreatePipelineState();

    m_pCommandList = LUL_GET_HARDWARE(m_pHardware)->CreateCommandList(m_pCommandAllocator);
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::RecordCommands()
{
    // Command list allocators can only be reset when the associated 
    // command lists have finished execution on the GPU; apps should use 
    // fences to determine GPU execution progress.
    L_THROW_IF_FAILED(m_pCommandAllocator->Reset());

    // However, when ExecuteCommandList() is called on a particular command 
    // list, that command list can then be reset at any time and must be before 
    // re-recording.
    L_THROW_IF_FAILED(m_pCommandList->Reset(m_pCommandAllocator.Get(), m_pPipelineState.Get()));

    auto vp = LUL_GET_HARDWARE(m_pHardware)->GetViewport();
    auto sc = LUL_GET_HARDWARE(m_pHardware)->GetScissorRect();

    // Set necessary state.
    m_pCommandList->SetGraphicsRootSignature(LUL_GET_MEMORY(m_pMemory)->GetRootSig().Get());
    m_pCommandList->RSSetViewports(1, &vp);
    m_pCommandList->RSSetScissorRects(1, &sc);

    // Indicate that the back buffer will be used as a render target.
    auto b = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetRenderTarget();
    m_pCommandList->ResourceBarrier(1, &b);

    auto rtvHandle = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetRtvHandle();
    m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Record commands.
    const float clearColor[] = { 0.901660f, 0.6f, 0.082352f, 1.0f };
    m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
    m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 
    // Create the vertex buffer.
    static ComPtr<ID3D12Resource> vbv = {};
    static D3D12_VERTEX_BUFFER_VIEW vbview;

    // Define the geometry for a triangle.
    Vertex triangleVertices[] =
    {
        { { 0.0f, 0.56f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.5f, 0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { 0.20f, 0.999f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, 0.5f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
        { { -0.20f, 0.999f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
        { { 0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
        { { -0.25f, -0.25f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
    };
    const UINT vertexBufferSize = sizeof(triangleVertices);

    // Note: using upload heaps to transfer static data like vert buffers is not 
    // recommended. Every time the GPU needs it, the upload heap will be marshalled 
    // over. Please read up on Default Heap usage. An upload heap is used here for 
    // code simplicity and because there are very few verts to actually transfer.
    auto aa = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    auto bb = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    L_THROW_IF_FAILED(LUL_GET_HARDWARE(m_pHardware)->GetDevice()->CreateCommittedResource(
        &aa,
        D3D12_HEAP_FLAG_NONE,
        &bb,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&vbv)));

    // Copy the triangle data to the vertex buffer.
    UINT8* pVertexDataBegin;
    CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
    L_THROW_IF_FAILED(vbv->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
    memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
    vbv->Unmap(0, nullptr);

    // Initialize the vertex buffer view.
    vbview.BufferLocation = vbv->GetGPUVirtualAddress();
    vbview.StrideInBytes = sizeof(Vertex);
    vbview.SizeInBytes = vertexBufferSize;
    
    m_pCommandList->IASetVertexBuffers(0, 3, &vbview);
    m_pCommandList->DrawInstanced(9, 3, 0, 0);
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::CloseCommandLine()

{    // Indicate that the back buffer will now be used to present.
    auto pb = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetPresentTarget();
    m_pCommandList->ResourceBarrier(1, &pb);

    L_THROW_IF_FAILED(m_pCommandList->Close());

    ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
    m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // Present the frame
    LUL_GET_SWAPCHAIN(m_pSwapChain)->Present();
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::Signal(ID3D12Fence* pFence, const uint64_t uValue) const
{
	L_THROW_IF_FAILED(m_pCommandQueue->Signal(pFence, uValue));
}
