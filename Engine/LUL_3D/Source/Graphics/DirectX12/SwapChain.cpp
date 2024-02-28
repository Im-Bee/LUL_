#include "LUL_3D.hpp"

#include "SwapChain.hpp"

using Microsoft::WRL::ComPtr;

// Swapchain -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::SwapChain::Initialize(
	Microsoft::WRL::ComPtr<IDXGIFactory> factory,
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::IUnknown> hardware,
	std::shared_ptr<const LUL_::IUnknown> memory,
	std::shared_ptr<const LUL_::IUnknown> commands)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::SwapChain | %p", this);
	m_pFactory = factory;

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

	m_pFactory.~ComPtr();
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

		m_pRenderTargets[i] = LUL_GET_HARDWARE(m_pHardware)->CreateRtv(cpuDescriptor, m_uRtvDescriptorSize);
	}
}
