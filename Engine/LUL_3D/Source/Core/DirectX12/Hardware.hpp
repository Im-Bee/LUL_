#pragma once

#ifdef _WIN64
namespace LUL_::DX12
{
	class LUL_EXPORT Hardware
		: public LUL_::IRendererComponent
	{
		LUL_IUNKNOWN_IMPL(Hardware);

	private:
		Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter = Microsoft::WRL::ComPtr<IDXGIAdapter>(nullptr);
		Microsoft::WRL::ComPtr<ID3D12Device> m_pDevice = Microsoft::WRL::ComPtr<ID3D12Device>(nullptr);

		CD3DX12_VIEWPORT m_ViewPort = CD3DX12_VIEWPORT();
		uint64_t m_uReservedMem = 0;

	public:
		Hardware() = default;
		~Hardware() = default;

	public:
		void Initialize(
			Microsoft::WRL::ComPtr<IDXGIFactory> factory,
			IRenderer const* const renderer,
			std::shared_ptr<LUL_::IRendererComponent> swapchain,
			std::shared_ptr<LUL_::IRendererComponent> memory,
			std::shared_ptr<LUL_::IRendererComponent> commands);

		void EndCreation();

		// Creation methods ----------------------------------------------------------
	public:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue() const;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator() const;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateDirectCommandList(
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator) const;

		Microsoft::WRL::ComPtr<IDXGISwapChain> CreateSwapChain() const;

		void CreateRtvDescriptorHeap(
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& pDescriptor,
			uint32_t& uDescriptorSize,
			const uint32_t& count) const;

		void CreateRtv(
			Microsoft::WRL::ComPtr<ID3D12Resource> target,
			CD3DX12_CPU_DESCRIPTOR_HANDLE& descriptorHandle,
			const uint32_t& uDescriptorSize) const;

		Microsoft::WRL::ComPtr<ID3D12RootSignature> CreateRootSignature() const;

		Microsoft::WRL::ComPtr<ID3D12PipelineState> CreatePipelineState() const;

		Microsoft::WRL::ComPtr<ID3D12Fence> CreateFence() const;

		Microsoft::WRL::ComPtr<ID3D12Resource> CreateResource(
			CD3DX12_HEAP_PROPERTIES& properites,
			D3D12_HEAP_FLAGS heapFlags,
			CD3DX12_RESOURCE_DESC& description,
			D3D12_RESOURCE_STATES initialState) const;

		// Setters ---------------------------------------------------------------------
	public:
		void SetViewport(CD3DX12_VIEWPORT n) { m_ViewPort = n; }

		// Getters ---------------------------------------------------------------------
	public:
		const CD3DX12_VIEWPORT& GetViewport() const { return m_ViewPort; }

	private:
		void CreateAdapter();
		void CreateDevice();

	private:
		Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory = Microsoft::WRL::ComPtr<IDXGIFactory>(nullptr);

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<LUL_::IRendererComponent> m_pSwapChain = std::shared_ptr<LUL_::IRendererComponent>(nullptr);
		std::shared_ptr<LUL_::IRendererComponent> m_pMemory = std::shared_ptr<LUL_::IRendererComponent>(nullptr);
		std::shared_ptr<LUL_::IRendererComponent> m_pCommands = std::shared_ptr<LUL_::IRendererComponent>(nullptr);
	};
}
#endif // _WIN64