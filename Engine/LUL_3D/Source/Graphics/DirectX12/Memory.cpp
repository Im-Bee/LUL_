#include "LUL_3D.hpp"

#include "Memory.hpp"

// ReservedMemory --------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::ReservedMemory::DescribeBuffer()
{
	switch (m_BufferType)
	{
		case (MeshBuffer) :
		{
				

			break;
		}
		default:
		{
			L_LOG(L_WARNING, L"Buffer type was invalid | %p", this);
			return;
		}
	}
}

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
void LUL_::Graphics::DX12::Memory::InitializeAssets()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Memory::InitializeAssets", this);

	m_pRootSignature = LUL_GET_HARDWARE(m_pHardware)->CreateRootSignature();
}

// -----------------------------------------------------------------------------
LUL_::Graphics::DX12::ReservedMemory LUL_::Graphics::DX12::Memory::ReserveMemory(const uint32_t bufferSize, BufferType type) const
{
	ReservedMemory newMem = ReserveMemory(bufferSize, type);



	return newMem;
}

