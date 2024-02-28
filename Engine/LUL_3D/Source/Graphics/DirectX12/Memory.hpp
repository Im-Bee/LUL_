#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT Memory
		: public LUL_::IUnknown
	{

		LUL_IUNKNOWN_IMPL(Memory);

	private:



	public:

		Memory() = default;

		~Memory() = default;

	public:

		void Initialize(
			Microsoft::WRL::ComPtr<IDXGIFactory> factory,
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::IUnknown> hardware,
			std::shared_ptr<const LUL_::IUnknown> swapchain,
			std::shared_ptr<const LUL_::IUnknown> commands);

	private:

		Microsoft::WRL::ComPtr<IDXGIFactory> m_pFactory = Microsoft::WRL::ComPtr<IDXGIFactory>(nullptr);

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this class
		std::shared_ptr<const LUL_::IUnknown> m_pHardware = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pSwapChain = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pCommands = std::shared_ptr<const LUL_::IUnknown>(nullptr);

	};
}
#endif // _WIN64