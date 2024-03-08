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

		Microsoft::WRL::ComPtr<IDXGIResource> m_pResource = Microsoft::WRL::ComPtr<IDXGIResource>(nullptr);
		D3D12_VERTEX_BUFFER_VIEW m_BufferView = {};

	public:

		ReservedMemory() = delete;

		explicit ReservedMemory(const BufferType type)
			: m_BufferType(type)
		{
			DescribeBuffer();
		}

		~ReservedMemory() = default;

	public:


	private:

		void DescribeBuffer();

	private:

		const BufferType m_BufferType;

		CD3DX12_HEAP_PROPERTIES m_HeapProperites = {};
		CD3DX12_RESOURCE_DESC m_ResourceDesc = {};
	};

	class LUL_EXPORT Memory
		: public LUL_::Graphics::IRendererComponent
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
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> hardware,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> swapchain,
			std::shared_ptr<const LUL_::Graphics::IRendererComponent> commands);

		void InitializeAssets();

	public:

		ReservedMemory ReserveMemory(const uint32_t bufferSize, BufferType type) const;

	public:

		// Getters ---------------------------------------------------------------------

		Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSig() const { return m_pRootSignature; }

	private:

		const IRenderer* m_pRenderer = nullptr; // Renderer should be alive through the whole life cycle of this object
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pHardware = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pSwapChain = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);
		std::shared_ptr<const LUL_::Graphics::IRendererComponent> m_pCommands = std::shared_ptr<const LUL_::Graphics::IRendererComponent>(nullptr);

	};
}
#endif // _WIN64