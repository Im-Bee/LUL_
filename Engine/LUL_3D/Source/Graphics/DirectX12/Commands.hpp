#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT Commands
		: public LUL_::Graphics::IRendererComponent
	{

		LUL_IUNKNOWN_IMPL(Commands);

	private:

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_pCommandQueue = Microsoft::WRL::ComPtr<ID3D12CommandQueue>(nullptr);
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_pCommandAllocator = Microsoft::WRL::ComPtr<ID3D12CommandAllocator>(nullptr);

		CD3DX12_RECT m_ScissorRect = CD3DX12_RECT();

		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pPipelineState = Microsoft::WRL::ComPtr<ID3D12PipelineState>(nullptr);

	private:

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList = Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>(nullptr);

	public:

		Commands() = default;

		~Commands() = default;

	public:

		void Initialize(
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> hardware,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> swapchain,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> memory);

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
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pHardware = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pSwapChain = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pMemory = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);

	};
}
#endif // _WIN64