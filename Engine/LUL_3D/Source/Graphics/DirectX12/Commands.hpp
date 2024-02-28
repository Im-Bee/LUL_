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

	private:

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_pCommandList = Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>(nullptr);

	public:

		Commands() = default;

		~Commands() = default;

	public:

		void Initialize(
			Microsoft::WRL::ComPtr<IDXGIFactory> factory,
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::IUnknown> hardware,
			std::shared_ptr<const LUL_::IUnknown> swapchain,
			std::shared_ptr<const LUL_::IUnknown> memory);

	public:

		// Getters ---------------------------------------------------------------------

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> GetCommandQueue() { return m_pCommandQueue; }

	private:



	private:

		Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory = Microsoft::WRL::ComPtr<IDXGIFactory>(nullptr);

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this class
		std::shared_ptr<const LUL_::IUnknown> m_pHardware = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pSwapChain = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pMemory = std::shared_ptr<const LUL_::IUnknown>(nullptr);

	};
}
#endif // _WIN64