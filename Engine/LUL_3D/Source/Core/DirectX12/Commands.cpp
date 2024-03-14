#include "LUL_3D.hpp"

#include "Commands.hpp"

using Microsoft::WRL::ComPtr;

// Commands -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Commands::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<LUL_::IRendererComponent> hardware,
	std::shared_ptr<LUL_::IRendererComponent> swapchain,
	std::shared_ptr<LUL_::IRendererComponent> memory)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::DX12::Commands | %p", this);

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

	m_pMainCommandQueue = LUL_GET_HARDWARE(m_pHardware)->CreateCommandQueue();
	m_pMainCommandAllocator = LUL_GET_HARDWARE(m_pHardware)->CreateCommandAllocator();
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Commands::InitializePipelineState()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize assets LUL_::DX12::Commands | %p", this);

	m_pPipelineState = LUL_GET_HARDWARE(m_pHardware)->CreatePipelineState();

	m_pMainCommandList = LUL_GET_HARDWARE(m_pHardware)->CreateDirectCommandList(m_pMainCommandAllocator);


	m_MainScissorRect = CD3DX12_RECT(
		0,
		0,
		m_pRenderer->GetTarget()->GetWindowDimensions().x,
		m_pRenderer->GetTarget()->GetWindowDimensions().y);
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Commands::BeginVerticesProcessing()
{
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	L_THROW_IF_FAILED(m_pMainCommandAllocator->Reset());

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	L_THROW_IF_FAILED(m_pMainCommandList->Reset(m_pMainCommandAllocator.Get(), m_pPipelineState.Get()));

	auto vp = LUL_GET_HARDWARE(m_pHardware)->GetViewport();
	auto sc = m_MainScissorRect;

	// Set necessary state.
	m_pMainCommandList->SetGraphicsRootSignature(LUL_GET_MEMORY(m_pMemory)->GetRootSig().Get());
	m_pMainCommandList->RSSetViewports(1, &vp);
	m_pMainCommandList->RSSetScissorRects(1, &sc);

	// Indicate that the back buffer will be used as a render target.
	auto b = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetRenderTarget();
	m_pMainCommandList->ResourceBarrier(1, &b);

	auto rtvHandle = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetRtvHandle();
	m_pMainCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.901660f, 0.6f, 0.082352f, 1.0f };
	m_pMainCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_pMainCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Commands::ProcessVertices()
{
	for (auto& i : LUL_GET_MEMORY(m_pMemory)->GetReservedMemory())
	{
		if (i->GetType() != MeshBuffer)
			continue;

		m_pMainCommandList->IASetVertexBuffers(0, 1, &i->GetView());
		m_pMainCommandList->DrawInstanced(i->GetDataCount(), 1, 0, 0);
	}
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Commands::CloseVerticesProcessing()
{
	// Indicate that the back buffer will now be used to present.
	auto pb = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetPresentTarget();
	m_pMainCommandList->ResourceBarrier(1, &pb);

	L_THROW_IF_FAILED(m_pMainCommandList->Close());

	ID3D12CommandList* ppCommandLists[] = { m_pMainCommandList.Get() };
	m_pMainCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame
	LUL_GET_SWAPCHAIN(m_pSwapChain)->Present();
}

// -----------------------------------------------------------------------------
void LUL_::DX12::Commands::Signal(ID3D12Fence* pFence, const uint64_t uValue) const
{
	L_THROW_IF_FAILED(m_pMainCommandQueue->Signal(pFence, uValue));
}
