#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT SwapChain
		: public LUL_::Graphics::IRendererComponent
	{

		LUL_IUNKNOWN_IMPL(SwapChain);

	private:

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = Microsoft::WRL::ComPtr<IDXGISwapChain>(nullptr);

		static const uint32_t m_uFrameCount = 2;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRtvDescriptorHeap = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>(nullptr);
		uint32_t m_uRtvDescriptorSize = 0;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_pRenderTargets[m_uFrameCount] = {};

		Microsoft::WRL::ComPtr<ID3D12Fence> m_pFence = Microsoft::WRL::ComPtr<ID3D12Fence>(nullptr);
		HANDLE m_FenceEvent = INVALID_HANDLE_VALUE;
		uint64_t m_uFrameIndex = -1;
		uint64_t m_uFenceValue = -1;

	public:

		SwapChain() = default;

		~SwapChain();

	public:

		void Initialize(
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> hardware,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> memory,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> commands);

		void InitializeFence();

	public:

		void Present() const;

		void WaitForPrevious();

	public:

		// Getters ---------------------------------------------------------------------

		static const uint32_t& GetFrameCount() { return m_uFrameCount; }

		CD3DX12_RESOURCE_BARRIER GetRenderTarget() const;

		CD3DX12_RESOURCE_BARRIER GetPresentTarget() const;

		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

	private:

		void CreateRtvs();

	private:

		Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory = Microsoft::WRL::ComPtr<IDXGIFactory>(nullptr);

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pHardware = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pMemory = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pCommands = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);

	};
}
#endif // _WIN64