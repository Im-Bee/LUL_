#include "LUL_3D.hpp"

#include "Memory.hpp"

// ReservedMemory --------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::DX12::ReservedMemory::ReservedMemory(
	Microsoft::WRL::ComPtr<ID3D12Resource> ptr,
	D3D12_VERTEX_BUFFER_VIEW& mem,
	CD3DX12_HEAP_PROPERTIES& prop,
	CD3DX12_RESOURCE_DESC& desc)
	: m_pBuffer(ptr),
	m_BufferView(std::move(mem)),
	m_Properties(std::move(prop)),
	m_Desc(std::move(desc))
{
	void* pData;
	CD3DX12_RANGE readRange(0, 0);

	m_pBuffer->Map(0, &readRange, &pData);

	m_pDataBegin = reinterpret_cast<uint8_t*>(pData);
	m_pDataCur = m_pDataBegin;
	m_pDataEnd = m_pDataBegin + desc.Width;
}

// -----------------------------------------------------------------------------
void LUL_::DX12::ReservedMemory::Upload(
	const void* pData, 
	uint64_t uBytesPerData, 
	uint32_t uDataCount, 
	uint32_t uAlignment, 
	uint32_t* uByteOffset)
{
	uint64_t uByteSize = uBytesPerData * uDataCount;

	L_THROW_IF_FAILED(SuballocateFromBuffer(uByteSize, uAlignment));

	*uByteOffset = uint32_t(m_pDataCur - m_pDataBegin);
	memcpy(m_pDataCur, pData, uByteSize);
	m_pDataCur += uByteSize;
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
HRESULT LUL_::DX12::ReservedMemory::SuballocateFromBuffer(uint64_t uSize, uint64_t uAlign)
{
	m_pDataCur = reinterpret_cast<uint8_t*>(Align(
		reinterpret_cast<uint64_t>(m_pDataCur),
		uAlign));

	return (m_pDataCur + uSize > m_pDataEnd) ? E_INVALIDARG : S_OK;
}

// -----------------------------------------------------------------------------
uint64_t LUL_::DX12::ReservedMemory::Align(uint64_t uLocation, uint64_t uAlign)
{
	if ((0 == uAlign) || (uAlign & (uAlign - 1)))
	{
		throw Exceptions::Internal(LUL_EXCPT_HELPER());
	}

	return ((uLocation + (uAlign - 1)) & ~(uAlign - 1));
}

// Memory ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Memory::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<LUL_::IRendererComponent> hardware, 
	std::shared_ptr<LUL_::IRendererComponent> swapchain,
	std::shared_ptr<LUL_::IRendererComponent> commands)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::DX12::Memory | %p", this);
	
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
void LUL_::DX12::Memory::InitializeRootSignature()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::DX12::Memory::InitializeAssets | %p", this);

	m_pRootSignature = LUL_GET_HARDWARE(m_pHardware)->CreateRootSignature();
}

// -----------------------------------------------------------------------------
std::shared_ptr<LUL_::DX12::ReservedMemory> LUL_::DX12::Memory::ReserveMemory(
	const uint32_t bufferSize, 
	const BufferType type)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::DX12::Memory::ReserveMemory | %p", this);

	D3D12_VERTEX_BUFFER_VIEW mem;
	CD3DX12_HEAP_PROPERTIES props; 
	CD3DX12_RESOURCE_DESC desc;
	Microsoft::WRL::ComPtr<ID3D12Resource> p;

	switch (type)
	{
		case (MeshBuffer):
		{
			props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
			p = LUL_GET_HARDWARE(m_pHardware)->CreateResource(
				props,
				D3D12_HEAP_FLAG_NONE,
				desc,
				D3D12_RESOURCE_STATE_GENERIC_READ);

			break;
		}
		default:
		{
			L_LOG(L_ERROR, L"LUL_::DX12::Memory::ReserveMemory unknown memory type");
		}
	}

	mem.BufferLocation = p->GetGPUVirtualAddress();
	mem.StrideInBytes = sizeof(Vertex);
	mem.SizeInBytes = bufferSize;

	m_vAllReservedMemory.push_back(std::make_shared<ReservedMemory>(
		p,
		mem, 
		props,
		desc));

	return m_vAllReservedMemory.back();
}