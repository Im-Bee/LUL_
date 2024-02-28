#include "LUL_3D.hpp"

#include "Memory.hpp"

// Memory ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Memory::Initialize(
	Microsoft::WRL::ComPtr<IDXGIFactory> factory, 
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::IUnknown> hardware, 
	std::shared_ptr<const LUL_::IUnknown> swapchain,
	std::shared_ptr<const LUL_::IUnknown> commands)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::Memory | %p", this);
	m_pFactory = factory;

	m_pRenderer = renderer;
	if (hardware->GetClass() != DX12::Hardware::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pHardware = hardware;
	if (swapchain->GetClass() != DX12::SwapChain::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pSwapChain = swapchain;
	if (commands->GetClass() != DX12::Commands::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pCommands = commands;

	m_pFactory.~ComPtr();
}
