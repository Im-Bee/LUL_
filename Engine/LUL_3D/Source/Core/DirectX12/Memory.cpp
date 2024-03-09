#include "LUL_3D.hpp"

#include "Memory.hpp"

// ReservedMemory --------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------


// Memory ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Memory::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::Graphics::IRendererComponent> hardware, 
	std::shared_ptr<const LUL_::Graphics::IRendererComponent> swapchain,
	std::shared_ptr<const LUL_::Graphics::IRendererComponent> commands)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::Memory | %p", this);
	
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
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Memory::InitializeRootSignature()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Memory::InitializeAssets | %p", this);

	m_pRootSignature = LUL_GET_HARDWARE(m_pHardware)->CreateRootSignature();
}

// -----------------------------------------------------------------------------
std::shared_ptr<LUL_::Graphics::DX12::ReservedMemory> LUL_::Graphics::DX12::Memory::ReserveMemory(
	const uint32_t bufferSize, 
	const BufferType type)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Memory::ReserveMemory | %p", this);

	Microsoft::WRL::ComPtr<IDXGIResource> p = 0;
	D3D12_VERTEX_BUFFER_VIEW mem;

	m_vAllReservedMemory.push_back(std::make_shared<ReservedMemory>(p, mem));

	return m_vAllReservedMemory.back();
}

