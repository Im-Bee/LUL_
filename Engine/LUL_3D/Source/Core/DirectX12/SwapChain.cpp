#include "LUL_3D.hpp"

#include "SwapChain.hpp"

using Microsoft::WRL::ComPtr;

// Swapchain -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::SwapChain::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<LUL_::Graphics::IRendererComponent> hardware,
	std::shared_ptr<LUL_::Graphics::IRendererComponent> memory,
	std::shared_ptr<LUL_::Graphics::IRendererComponent> commands)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::SwapChain | %p", this);

	m_pRenderer = renderer;
	if (hardware->GetClass() != DX12::Hardware::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pHardware = hardware;
	if (memory->GetClass() != DX12::Memory::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pMemory = memory;
	if (commands->GetClass() != DX12::Commands::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pCommands = commands;

	m_pSwapChain = LUL_GET_HARDWARE(m_pHardware)->CreateSwapChain();
	LUL_GET_HARDWARE(m_pHardware)->CreateRtvDescriptorHeap(
		m_pRtvDescriptorHeap,
		m_uRtvDescriptorSize,
		m_uFrameCount);

	CreateRtvs();
}

// -----------------------------------------------------------------------------
LUL_::Graphics::DX12::SwapChain::~SwapChain()
{
	WaitForPrevious();
	CloseHandle(m_FenceEvent);
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::SwapChain::InitializeFence()
{
	m_pFence = LUL_GET_HARDWARE(m_pHardware)->CreateFence();
	m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_FenceEvent == nullptr)
	{
		L_THROW_IF_FAILED(HRESULT_FROM_WIN32(GetLastError()));
	}
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::SwapChain::Present() const
{
	L_THROW_IF_FAILED(m_pSwapChain->Present(1, 0));
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::SwapChain::WaitForPrevious()
{
	const uint64_t fence = m_uFenceValue;
	LUL_GET_COMMANDS(m_pCommands)->Signal(m_pFence.Get(), m_uFenceValue);
	m_uFenceValue++;

	// Wait for previous frame
	if (m_pFence->GetCompletedValue() < fence)
	{
		L_THROW_IF_FAILED(m_pFence->SetEventOnCompletion(fence, m_FenceEvent));
		WaitForSingleObject(m_FenceEvent, INFINITE);
	}

	ComPtr<IDXGISwapChain3> swapChain3;

	if (FAILED(m_pSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain3))))
	{
		throw Exceptions::Internal(LUL_EXCPT_HELPER());
	}

	m_uFrameIndex = swapChain3->GetCurrentBackBufferIndex();
}

// -----------------------------------------------------------------------------
CD3DX12_RESOURCE_BARRIER LUL_::Graphics::DX12::SwapChain::GetRenderTarget() const
{
	return CD3DX12_RESOURCE_BARRIER::Transition(
		m_pRenderTargets[m_uFrameIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATE_RENDER_TARGET);
}

// -----------------------------------------------------------------------------
CD3DX12_RESOURCE_BARRIER LUL_::Graphics::DX12::SwapChain::GetPresentTarget() const
{
	return CD3DX12_RESOURCE_BARRIER::Transition(
		m_pRenderTargets[m_uFrameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);
}

// -----------------------------------------------------------------------------
CD3DX12_CPU_DESCRIPTOR_HANDLE LUL_::Graphics::DX12::SwapChain::GetRtvHandle() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		static_cast<int32_t>(m_uFrameIndex),
		m_uRtvDescriptorSize);
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::SwapChain::CreateRtvs()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::SwapChain::CreateRtvs create");

	auto cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_pRtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (uint32_t i = 0; i < m_uFrameCount; ++i)
	{
		L_THROW_IF_FAILED(m_pSwapChain->GetBuffer(
			i, 
			IID_PPV_ARGS(&m_pRenderTargets[i])));

		L_THROW_IF_FAILED(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pRenderTargets[i])));
		LUL_GET_HARDWARE(m_pHardware)->CreateRtv(m_pRenderTargets[i], cpuDescriptor, m_uRtvDescriptorSize);
	}
}
