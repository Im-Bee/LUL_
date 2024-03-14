#pragma once

#ifdef _WIN64
namespace LUL_::DX12
{
	class ReservedMemory;

	typedef std::shared_ptr<LUL_::DX12::ReservedMemory> GpuBasedBuffer;

	enum LUL_EXPORT BufferType
	{
		Unvalid = 0x00,
		MeshBuffer = 0x10
	};

	enum LUL_EXPORT BufferOffset
	{
		Begin = 0x01,
		Cur = 0x10
	};

	class LUL_EXPORT ReservedMemory
	{
		friend LUL_::IRendererComponent;

		Microsoft::WRL::ComPtr<ID3D12Resource> m_pBuffer = Microsoft::WRL::ComPtr<ID3D12Resource>(nullptr);
		D3D12_VERTEX_BUFFER_VIEW m_BufferView = {};
		BufferType m_Type = Unvalid;

	public:
		ReservedMemory() = delete;
		explicit ReservedMemory(
			Microsoft::WRL::ComPtr<ID3D12Resource> ptr,
			D3D12_VERTEX_BUFFER_VIEW& mem,
			CD3DX12_HEAP_PROPERTIES& prop,
			CD3DX12_RESOURCE_DESC& desc,
			BufferType type);
		~ReservedMemory() = default;

	public:
		void Upload(const void* pData,
			uint64_t uBytesPerData,
			uint32_t uDataCount,
			uint32_t uAlignment,
			BufferOffset offset);

		// Getters ---------------------------------------------------------------------
	public:
		BufferType GetType() { return m_Type; }

		D3D12_VERTEX_BUFFER_VIEW& GetView() { return m_BufferView; }

		uint32_t GetDataCount() { return m_uDataCount; }

	private:
		HRESULT SuballocateFromBuffer(uint64_t uSize, uint64_t uAlign);
		uint64_t Align(uint64_t uLocation, uint64_t uAlign);

	private:
		uint8_t* m_pDataBegin = nullptr;
		uint8_t* m_pDataCur = nullptr;
		uint8_t* m_pDataEnd = nullptr;

		uint32_t m_uDataCount = 0;

		CD3DX12_HEAP_PROPERTIES m_Properties;
		CD3DX12_RESOURCE_DESC m_Desc;
	};

	class LUL_EXPORT GpuBasedObject
	{
		LUL_::DX12::GpuBasedBuffer m_Buffer = LUL_::DX12::GpuBasedBuffer(nullptr);

	public:
		GpuBasedObject() = default;
		GpuBasedObject(const GpuBasedObject&) = default;
		GpuBasedObject(GpuBasedObject&&) = delete;
		~GpuBasedObject() = default;

	public:
		void SetBuffer(LUL_::DX12::GpuBasedBuffer b) { m_Buffer = b; }

	public:
		GpuBasedBuffer GetRawBuffer() { return m_Buffer; }
	};

	class LUL_EXPORT Memory
		: public LUL_::IRendererComponent
	{
		LUL_IUNKNOWN_IMPL(Memory);

	private:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature = Microsoft::WRL::ComPtr<ID3D12RootSignature>(nullptr);

		std::vector<std::shared_ptr<ReservedMemory>> m_vAllReservedMemory = std::vector<std::shared_ptr<ReservedMemory>>();

	public:
		Memory() = default;
		~Memory() = default;

	public:
		void Initialize(
			IRenderer const* const renderer,
			std::shared_ptr<LUL_::IRendererComponent> hardware,
			std::shared_ptr<LUL_::IRendererComponent> swapchain,
			std::shared_ptr<LUL_::IRendererComponent> commands);

		void InitializeRootSignature();

	public:
		GpuBasedBuffer ReserveMemory(const uint32_t bufferSize, const BufferType type);

		// Getters ---------------------------------------------------------------------
	public:
		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSig() const { return m_pRootSignature; }

		std::vector<std::shared_ptr<ReservedMemory>>& GetReservedMemory() { return m_vAllReservedMemory; }

	private:
		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<LUL_::IRendererComponent> m_pHardware = std::shared_ptr<LUL_::IRendererComponent>(nullptr);
		std::shared_ptr<LUL_::IRendererComponent> m_pSwapChain = std::shared_ptr<LUL_::IRendererComponent>(nullptr);
		std::shared_ptr<LUL_::IRendererComponent> m_pCommands = std::shared_ptr<LUL_::IRendererComponent>(nullptr);
	};
}
#endif // _WIN64