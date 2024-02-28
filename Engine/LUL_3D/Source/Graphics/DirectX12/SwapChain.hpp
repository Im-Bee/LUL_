#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT SwapChain
		: public LUL_::IUnknown
	{

		LUL_IUNKNOWN_IMPL(SwapChain);

	private:

		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = Microsoft::WRL::ComPtr<IDXGISwapChain>(nullptr);

		static const uint32_t m_uFrameCount = 2;

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRtvDescriptorHeap = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>(nullptr);
		uint32_t m_uRtvDescriptorSize = 0;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_pRenderTargets[m_uFrameCount] = {};

	public:

		SwapChain() = default;

		~SwapChain() = default;

	public:

		void Initialize(
			Microsoft::WRL::ComPtr<IDXGIFactory> factory,
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::IUnknown> hardware,
			std::shared_ptr<const LUL_::IUnknown> memory,
			std::shared_ptr<const LUL_::IUnknown> commands);

	public:

		// Getters ---------------------------------------------------------------------

		static const uint32_t& GetFrameCount() { return m_uFrameCount; }

	private:

		void CreateRtvs();

	private:

		Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory = Microsoft::WRL::ComPtr<IDXGIFactory>(nullptr);

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this class
		std::shared_ptr<const LUL_::IUnknown> m_pHardware = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pMemory = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pCommands = std::shared_ptr<const LUL_::IUnknown>(nullptr);

	};
}
#endif // _WIN64