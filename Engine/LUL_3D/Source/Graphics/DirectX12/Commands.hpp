#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT Commands
		: public LUL_::IUnknown
	{

		LUL_IUNKNOWN_IMPL(Commands);

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCommandQueue = Microsoft::WRL::ComPtr<ID3D12CommandQueue>(nullptr);
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCommandAllocator = Microsoft::WRL::ComPtr<ID3D12CommandAllocator>(nullptr);

		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pPipelineState = Microsoft::WRL::ComPtr<ID3D12PipelineState>(nullptr);

	private:

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList = Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>(nullptr);

	public:

		Commands() = default;

		~Commands() = default;

	public:

		void Initialize(
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::IUnknown> hardware,
			std::shared_ptr<const LUL_::IUnknown> swapchain,
			std::shared_ptr<const LUL_::IUnknown> memory);

		void InitializeAssets();

	public:

		void RecordCommands();

		void CloseCommandLine();

	public:

		void Signal(
			ID3D12Fence* pFence,
			const uint64_t uValue) const;

	public:

		// Getters ---------------------------------------------------------------------

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return m_pCommandQueue; }

	private:



	private:

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<const LUL_::IUnknown> m_pHardware = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pSwapChain = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pMemory = std::shared_ptr<const LUL_::IUnknown>(nullptr);

	};
}
#endif // _WIN64