#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT Hardware
		: public LUL_::IUnknown
	{

		LUL_IUNKNOWN_IMPL(Hardware);

	private:

		Microsoft::WRL::ComPtr<IDXGIAdapter> m_pAdapter = Microsoft::WRL::ComPtr<IDXGIAdapter>(nullptr);
		Microsoft::WRL::ComPtr<ID3D12Device> m_pDevice = Microsoft::WRL::ComPtr<ID3D12Device>(nullptr);

		CD3DX12_VIEWPORT m_ViewPort = CD3DX12_VIEWPORT();
		CD3DX12_RECT m_ScissorRect = CD3DX12_RECT();

		uint64_t m_uReservedMem = 0;

	public:

		Hardware() = default;

		~Hardware() = default;

	public:

		void Initialize(
			Microsoft::WRL::ComPtr<IDXGIFactory> factory,
			IRenderer const* const renderer,
			std::shared_ptr<LUL_::IUnknown> swapchain,
			std::shared_ptr<LUL_::IUnknown> memory,
			std::shared_ptr<LUL_::IUnknown> commands);

		void EndCreation();

	public:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue() const;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator() const;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CreateCommandList(Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator) const;

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



	public:

		// Setters ---------------------------------------------------------------------

		void SetViewport(CD3DX12_VIEWPORT n) { m_ViewPort = n; }

	public:

		// Getters ---------------------------------------------------------------------

		const CD3DX12_VIEWPORT& GetViewport() const { return m_ViewPort; }

		const CD3DX12_RECT& GetScissorRect() const { return m_ScissorRect; }

		Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() const { return m_pDevice; }

	private:

		void CreateAdapter();

		void CreateDevice();

	private:

		Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory = Microsoft::WRL::ComPtr<IDXGIFactory>(nullptr);

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<LUL_::IUnknown> m_pSwapChain = std::shared_ptr<LUL_::IUnknown>(nullptr);
		std::shared_ptr<LUL_::IUnknown> m_pMemory = std::shared_ptr<LUL_::IUnknown>(nullptr);
		std::shared_ptr<LUL_::IUnknown> m_pCommands = std::shared_ptr<LUL_::IUnknown>(nullptr);

	};
}
#endif // _WIN64