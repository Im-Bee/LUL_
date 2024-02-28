#include "LUL_3D.hpp"

#include "Commands.hpp"

// Commands -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::Initialize(
	Microsoft::WRL::ComPtr<IDXGIFactory> factory, 
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::IUnknown> hardware,
	std::shared_ptr<const LUL_::IUnknown> swapchain,
	std::shared_ptr<const LUL_::IUnknown> memory)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::Commands | %p", this);
	m_pFactory = factory;

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

	m_pFactory.~ComPtr();
}
