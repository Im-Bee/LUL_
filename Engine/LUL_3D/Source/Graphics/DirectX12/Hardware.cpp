#include "LUL_3D.hpp"

#include "Hardware.hpp"

using Microsoft::WRL::ComPtr;

#define LUL_DX12_LOG_CATEGORY(dest, cat)		\
case (cat):										\
{ dest = #cat; break; }

// -----------------------------------------------------------------------------
void LogDX12(
	D3D12_MESSAGE_CATEGORY Category,
    D3D12_MESSAGE_SEVERITY Severity,
    D3D12_MESSAGE_ID ID,
    LPCSTR pDescription,
    void* pContext)
{
    LUL_::Severity sev = {};
    std::string categoryStr = "";

    switch (Severity)
    {
		case (D3D12_MESSAGE_SEVERITY_CORRUPTION):
		case (D3D12_MESSAGE_SEVERITY_ERROR):
		{
			sev = L_ERROR;
			break;
		}
		case (D3D12_MESSAGE_SEVERITY_WARNING):
		{
			sev = L_WARNING;
			break;
		}
		case (D3D12_MESSAGE_SEVERITY_INFO):
		case (D3D12_MESSAGE_SEVERITY_MESSAGE):
		{
			sev = L_INFO;
			break;
		}
		default:
		{
			sev = L_ERROR;
			break;
		}
    }

	switch (Category)
	{
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_APPLICATION_DEFINED);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_MISCELLANEOUS);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_INITIALIZATION);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_CLEANUP);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_COMPILATION);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_STATE_CREATION);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_STATE_SETTING);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_STATE_GETTING);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_RESOURCE_MANIPULATION);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_EXECUTION);
		LUL_DX12_LOG_CATEGORY(categoryStr, D3D12_MESSAGE_CATEGORY_SHADER);
		default:
		{
			categoryStr = "???";
			break;
		}
	}

    L_LOG(
		sev,
        L"[%S] [ID %d] %S",
        categoryStr.c_str(),
        ID,
        pDescription);
}

// Hardware --------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Hardware::Initialize(
	Microsoft::WRL::ComPtr<IDXGIFactory> factory,
	IRenderer const* const renderer,
	std::shared_ptr<LUL_::IUnknown> swapchain,
	std::shared_ptr<LUL_::IUnknown> memory,
	std::shared_ptr<LUL_::IUnknown> commands)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::Hardware | %p", this);
	m_pFactory = factory;

	m_pRenderer = renderer;
	if (swapchain->GetClass() != DX12::SwapChain::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pSwapChain = swapchain;
	if (memory->GetClass() != DX12::Memory::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pMemory = memory;
	if (commands->GetClass() != DX12::Commands::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pCommands = commands;

	CreateAdapter();
	CreateDevice();

	// Initialize debug message callback
	#ifdef _DEBUG
		ComPtr<ID3D12InfoQueue1> infoQueue = 0;
		m_pDevice->QueryInterface(IID_PPV_ARGS(&infoQueue));
		DWORD cookie = 0;
		if (infoQueue)
		{
			infoQueue->RegisterMessageCallback(
				LogDX12,
				D3D12_MESSAGE_CALLBACK_FLAG_NONE,
				nullptr,
				&cookie);
		}
		else
		{
			L_LOG(
				L_ERROR, 
				L"LUL_::Graphics::DX12::Hardware::Initialize couldn't create infoQueue");
		}

		if (!cookie)
		{
			throw Exceptions::Internal(LUL_EXCPT_HELPER());
		}
	#endif // _DEBUG

	// Set viewport dimensions
	m_ViewPort.Width = static_cast<FLOAT>(m_pRenderer->GetTarget()->GetWindowDimensions().x);
	m_ViewPort.Height = static_cast<FLOAT>(m_pRenderer->GetTarget()->GetWindowDimensions().y);

	m_pFactory.~ComPtr();
}

// -----------------------------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D12CommandQueue> LUL_::Graphics::DX12::Hardware::CreateCommandQueue() const
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Hardware Create command queue");

	ComPtr<ID3D12CommandQueue> commandQueue = 0;

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	if (FAILED(m_pDevice->CreateCommandQueue(
		&queueDesc,
		IID_PPV_ARGS(&commandQueue))))
	{
		throw Exceptions::FeatureNotSupported();
	}
	
	return commandQueue;
}

Microsoft::WRL::ComPtr<ID3D12CommandAllocator> LUL_::Graphics::DX12::Hardware::CreateCommandAllocator() const
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Hardware Create command allocator");

	ComPtr<ID3D12CommandAllocator> cmdAlloc = 0;

	L_THROW_IF_FAILED(m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc)));

	return cmdAlloc;
}

// -----------------------------------------------------------------------------
Microsoft::WRL::ComPtr<IDXGISwapChain> LUL_::Graphics::DX12::Hardware::CreateSwapChain() const
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Hardware Create command queue");

	DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
	swapDesc.BufferCount = static_cast<DX12::SwapChain*>(m_pSwapChain.get())->GetFrameCount();
	swapDesc.Width = static_cast<uint32_t>(GetViewport().Width);
	swapDesc.Height = static_cast<uint32_t>(GetViewport().Height);
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	ComPtr<IDXGIFactory2> factory2 = 0;
	ComPtr<IDXGISwapChain1> swapChain1 = 0;

	if (SUCCEEDED(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory2))))
	{
		factory2->CreateSwapChainForHwnd(
			static_cast<DX12::Commands*>(m_pCommands.get())->GetCommandQueue().Get(),
			LUL_GET_WINDOW_HANDLE(m_pRenderer->GetTarget()->GetHandle()),
			&swapDesc,
			nullptr,
			nullptr,
			&swapChain1);
	}

	if (!swapChain1.Get())
	{
		throw Exceptions::FeatureNotSupported(LUL_EXCPT_HELPER());
	}

	return swapChain1;
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Hardware::CreateRtvDescriptorHeap(
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>& pDescriptor,
	uint32_t& uDescriptorSize, 
	const uint32_t& count) const
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Hardware Create descriptor heap");

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	heapDesc.NumDescriptors = count;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	L_THROW_IF_FAILED(m_pDevice->CreateDescriptorHeap(
		&heapDesc, 
		IID_PPV_ARGS(&pDescriptor)));

	uDescriptorSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
}

Microsoft::WRL::ComPtr<ID3D12Resource> LUL_::Graphics::DX12::Hardware::CreateRtv(
	CD3DX12_CPU_DESCRIPTOR_HANDLE& descriptorHandle, 
	const uint32_t& uDescriptorSize) const
{
	L_LOG(L_INFO, L"LUL_::Graphics::DX12::Hardware Create render target view");

	ComPtr<ID3D12Resource> rtv = 0;
	m_pDevice->CreateRenderTargetView(rtv.Get(), nullptr, descriptorHandle);
	descriptorHandle.Offset(1, uDescriptorSize);

	return rtv;
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Hardware::CreateAdapter()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Create LUL_::Graphics::DX12::Hardware::CreateAdapter | %p", this);

	if (!m_pFactory.Get())
		throw Exceptions::InvalidArg(LUL_EXCPT_HELPER());

	ComPtr<IDXGIAdapter1> adapter1 = 0;
	ComPtr<IDXGIFactory1> factory1 = 0;
	ComPtr<IDXGIFactory6> factory6 = 0;
	DXGI_ADAPTER_DESC1 desc1 = {};

	if (SUCCEEDED(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory6))))
	{
		for (uint32_t adtprI = 0;
			SUCCEEDED(factory6->EnumAdapterByGpuPreference(
				adtprI,
				DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
				IID_PPV_ARGS(&adapter1)));
				++adtprI)
		{
			adapter1->GetDesc1(&desc1);

			if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			if (SUCCEEDED(D3D12CreateDevice(
				adapter1.Get(),
				D3D_FEATURE_LEVEL_12_0,
				_uuidof(ID3D12Device),
				nullptr)))
			{
				break;
			}
		}
	}

	if (!adapter1.Get())
	{
		L_THROW_IF_FAILED(m_pFactory->QueryInterface(IID_PPV_ARGS(&factory1)));

		for (uint32_t adtprI = 0;
			SUCCEEDED(factory1->EnumAdapters1(
				adtprI,
				&adapter1));
			++adtprI)
		{
			adapter1->GetDesc1(&desc1);

			if (desc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				continue;

			if (SUCCEEDED(D3D12CreateDevice(
				adapter1.Get(),
				D3D_FEATURE_LEVEL_12_0,
				_uuidof(ID3D12Device),
				nullptr)))
			{
				break;
			}
		}
	}

	m_pAdapter = adapter1.Detach();

	if (!m_pAdapter.Get())
	{
		throw LUL_::Exceptions::Exception("Couldn't find DirectX12 comaptible device");
	}
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Hardware::CreateDevice()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Create LUL_::Graphics::DX12::Hardware::CreateDevice | %p", this);

	if (!m_pFactory.Get())
		throw Exceptions::InvalidArg(LUL_EXCPT_HELPER());

	L_THROW_IF_FAILED(D3D12CreateDevice(
		m_pAdapter.Get(),
		D3D_FEATURE_LEVEL_12_0,
		IID_PPV_ARGS(&m_pDevice)));
}
