#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT Memory
		: public LUL_::IUnknown
	{

		LUL_IUNKNOWN_IMPL(Memory);

	private:

		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_pRootSignature = Microsoft::WRL::ComPtr<ID3D12RootSignature>(nullptr);

	public:

		Memory() = default;

		~Memory() = default;

	public:

		void Initialize(
			IRenderer const* const renderer,
			std::shared_ptr<const LUL_::IUnknown> hardware,
			std::shared_ptr<const LUL_::IUnknown> swapchain,
			std::shared_ptr<const LUL_::IUnknown> commands);

		void InitializeAssets();

	public:

		D3D12_VERTEX_BUFFER_VIEW AllocateVertices(Vertex const* const buffer, const uint32_t bufferSize);

	public:

		// Getters ---------------------------------------------------------------------

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSig() const { return m_pRootSignature; }

	private:

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<const LUL_::IUnknown> m_pHardware = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pSwapChain = std::shared_ptr<const LUL_::IUnknown>(nullptr);
		std::shared_ptr<const LUL_::IUnknown> m_pCommands = std::shared_ptr<const LUL_::IUnknown>(nullptr);

	};
}
#endif // _WIN64