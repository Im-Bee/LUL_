#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	enum LUL_EXPORT BufferType
	{
		MeshBuffer = 0x1
	};

	class LUL_EXPORT ReservedMemory
	{
	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> m_pBuffer = Microsoft::WRL::ComPtr<ID3D12Resource>(nullptr);
		D3D12_VERTEX_BUFFER_VIEW m_BufferView = {};

	public:

		ReservedMemory() = delete;

		explicit ReservedMemory(
			Microsoft::WRL::ComPtr<ID3D12Resource> ptr,
			D3D12_VERTEX_BUFFER_VIEW& mem,
			CD3DX12_HEAP_PROPERTIES& prop,
			CD3DX12_RESOURCE_DESC& desc);

		~ReservedMemory() = default;

	public:

		// Methods ---------------------------------------------------------------------

		void SendDataToUploadBuffer(const void* pData,
			uint64_t uBytesPerData,
			uint32_t uDataCount,
			uint32_t uAlignment,
			uint32_t* uByteOffset);

	private:

		HRESULT SuballocateFromBuffer(uint64_t uSize, uint64_t uAlign);
		uint64_t Align(uint64_t uLocation, uint64_t uAlign);

	private:

		uint8_t* m_pDataBegin = nullptr;
		uint8_t* m_pDataCur = nullptr;
		uint8_t* m_pDataEnd = nullptr;

		CD3DX12_HEAP_PROPERTIES m_Properties;
		CD3DX12_RESOURCE_DESC m_Desc;

	};

	class LUL_EXPORT Memory
		: public LUL_::Graphics::IRendererComponent
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
			std::shared_ptr<LUL_::Graphics::IRendererComponent> hardware,
			std::shared_ptr<LUL_::Graphics::IRendererComponent> swapchain,
			std::shared_ptr<LUL_::Graphics::IRendererComponent> commands);

		void InitializeRootSignature();

	public:

		// Methods ---------------------------------------------------------------------

		std::shared_ptr<ReservedMemory> ReserveMemory(const uint32_t bufferSize, const BufferType type);

	public:

		// Getters ---------------------------------------------------------------------

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSig() const { return m_pRootSignature; }

	private:

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<LUL_::Graphics::IRendererComponent> m_pHardware = std::shared_ptr<LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<LUL_::Graphics::IRendererComponent> m_pSwapChain = std::shared_ptr<LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<LUL_::Graphics::IRendererComponent> m_pCommands = std::shared_ptr<LUL_::Graphics::IRendererComponent>(nullptr);

	};
}
#endif // _WIN64