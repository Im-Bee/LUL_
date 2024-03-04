#include "LUL_3D.hpp"

#include "Memory.hpp"

// Memory ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Memory::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::IUnknown> hardware, 
	std::shared_ptr<const LUL_::IUnknown> swapchain,
	std::shared_ptr<const LUL_::IUnknown> commands)
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
void LUL_::Graphics::DX12::Memory::InitializeAssets()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Memory::InitializeAssets", this);

	m_pRootSignature = LUL_GET_HARDWARE(m_pHardware)->CreateRootSignature();
}

// -----------------------------------------------------------------------------
// D3D12_VERTEX_BUFFER_VIEW LUL_::Graphics::DX12::Memory::AllocateVertices(Vertex const* const buffer, const uint32_t bufferSize)
// {
// 
// 
// 
// 
// 
// 	return D3D12_VERTEX_BUFFER_VIEW();
// }
