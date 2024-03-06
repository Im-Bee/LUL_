#include "LUL_3D.hpp"

#include "Commands.hpp"

using Microsoft::WRL::ComPtr;

// Commands -------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::Initialize(
	IRenderer const* const renderer,
	std::shared_ptr<const LUL_::IUnknown> hardware,
	std::shared_ptr<const LUL_::IUnknown> swapchain,
	std::shared_ptr<const LUL_::IUnknown> memory)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize LUL_::Graphics::DX12::Commands | %p", this);

	m_pRenderer = renderer;
	if (hardware->GetClass() != DX12::Hardware::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pHardware = hardware;
	if (swapchain->GetClass() != DX12::SwapChain::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pSwapChain = swapchain;
	if (memory->GetClass() != DX12::Memory::GetClassId())
		throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER());
	m_pMemory = memory;

	m_pCommandQueue = LUL_GET_HARDWARE(m_pHardware)->CreateCommandQueue();
	m_pCommandAllocator = LUL_GET_HARDWARE(m_pHardware)->CreateCommandAllocator();
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::InitializeAssets()
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Initialize assets LUL_::Graphics::DX12::Commands | %p", this);

	m_pPipelineState = LUL_GET_HARDWARE(m_pHardware)->CreatePipelineState();

	m_pCommandList = LUL_GET_HARDWARE(m_pHardware)->CreateDirectCommandList(m_pCommandAllocator);
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::RecordCommands()
{
	// Command list allocators can only be reset when the associated 
	// command lists have finished execution on the GPU; apps should use 
	// fences to determine GPU execution progress.
	L_THROW_IF_FAILED(m_pCommandAllocator->Reset());

	// However, when ExecuteCommandList() is called on a particular command 
	// list, that command list can then be reset at any time and must be before 
	// re-recording.
	L_THROW_IF_FAILED(m_pCommandList->Reset(m_pCommandAllocator.Get(), m_pPipelineState.Get()));

	auto vp = LUL_GET_HARDWARE(m_pHardware)->GetViewport();
	auto sc = LUL_GET_HARDWARE(m_pHardware)->GetScissorRect();

	// Set necessary state.
	m_pCommandList->SetGraphicsRootSignature(LUL_GET_MEMORY(m_pMemory)->GetRootSig().Get());
	m_pCommandList->RSSetViewports(1, &vp);
	m_pCommandList->RSSetScissorRects(1, &sc);

	// Indicate that the back buffer will be used as a render target.
	auto b = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetRenderTarget();
	m_pCommandList->ResourceBarrier(1, &b);

	auto rtvHandle = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetRtvHandle();
	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.901660f, 0.6f, 0.082352f, 1.0f };
	m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_pCommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the vertex buffer.
	static ComPtr<ID3D12Resource> vbv = {};
	static D3D12_VERTEX_BUFFER_VIEW vbview;


	static float
		fRain1 = 0.0f,
		fRain2 = 0.5f,
		fRain3 = 0.25f;

	static float 
		fRA1 = 0.0008f,
		fRA2 = 0.0002f,
		fRA3 = 0.0005f;

	if (fRain1 >= 1.0f ||
		fRain1 <= 0.0f)
		fRA1 = -fRA1;
	if (fRain2 >= 1.0f ||
		fRain2 <= 0.0f)
		fRA2 = -fRA2;
	if (fRain3 >= 1.0f ||
		fRain3 <= 0.0f)
		fRA3 = -fRA3;

	fRain1 += fRA1;
	fRain2 += fRA2;
	fRain3 += fRA3;

	// Define the geometry for a triangle.
	Vertex triangleVertices[] =
	{
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },

		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },
		
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain2, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain3, 1.0f } },
		
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain2, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain2, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain2, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain2, fRain2, 1.0f } },
		
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		// ---------------------------------------------------------------------------

		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },

		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain2, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain3, 1.0f } },

		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain2, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },

		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain2, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain2, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain2, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		// -----------------------------------------------------------------------------

		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },

		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain2, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain3, 1.0f } },

		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain2, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },

		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain2, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain2, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain2, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } }
	};
	const UINT vertexBufferSize = sizeof(triangleVertices);

#if 1
	{
		using namespace DirectX;

		static const float fFov = 59.0f;
		static const float fAspectRatio = 1200.0f / 800.0f;
		static const float fNear = 0.001f;
		static const float fFar = 1000.0f;

		XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(
			XMConvertToRadians(fFov),
			fAspectRatio,
			fNear,
			fFar);

		static float fTheta = 1.0f;
		static float fThetaDirection = 0.05f;
		fTheta += fThetaDirection;

		static float fOffset = 1.0f;
		static float fOffDirection = 0.0001f;
		fOffset += fOffDirection;
		if (fOffset > 7.0f || fOffset <= 0.0f)
			fOffDirection = -fOffDirection;

		XMVECTOR 
			v,
			result;

		XMMATRIX 
			rotationZ,
			rotationX,
			rotationY,
			rotatedXZ,
			rotatedXZY,
			finished;

		float secondCubeOff = 0.0f;
		int i = 0;
		for (auto& tri : triangleVertices)
		{
			if (i >= 36)
				secondCubeOff = -3.5f;
			if (i >= 72)
				secondCubeOff = 3.5f;

			v.m128_f32[0] = tri.postion.x + secondCubeOff;
			v.m128_f32[1] = tri.postion.y + fOffset + secondCubeOff;
			v.m128_f32[2] = tri.postion.z + secondCubeOff;
			v.m128_f32[3] = tri.postion.w;
			
			// // Make rotations
			rotationZ = XMMatrixRotationZ(XMConvertToRadians(fTheta));
			rotationX = XMMatrixRotationX(XMConvertToRadians(fTheta));
			rotationY = XMMatrixRotationY(XMConvertToRadians(fTheta));
			
			// Rotate
			rotatedXZ = XMMatrixMultiply(rotationX, rotationZ);
			rotatedXZY = XMMatrixMultiply(rotatedXZ, rotationY);
			
			// Project
			finished = XMMatrixMultiply(rotatedXZY, projection);
			
			// Calculate result
			result = XMVector4Transform(v, finished);

 			if (result.m128_f32[2] < 0.0f)
 				result.m128_f32[2] = -result.m128_f32[2];

			// Scale down
			tri.postion.x = result.m128_f32[0] * 0.1f;
			tri.postion.y = result.m128_f32[1] * 0.1f;
			tri.postion.z = result.m128_f32[2] * 0.1f;
			tri.postion.w = 1.0f;  // result.m128_f32[3] * 0.1f;
			// L_LOG(L_INFO, L"%f %f %f %f", tri[0], tri[1], tri[2], tri[3]);
			++i;
		}
	}
#else
	{
		static const float fNear = 0.1f;
		static const float fFar = 10000.0f;
		static const float fFov = 90.0f;
		static const float fAspectRatio = 800.0f / 1200.0f;
		static const float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

		L_MatFloat4x4 matProj;
		matProj.m[0][0] = fAspectRatio * fFovRad;
		matProj.m[1][1] = fFovRad;
		matProj.m[2][2] = fFar / (fFar - fNear);
		matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		matProj.m[2][3] = 1.0f;
		matProj.m[3][3] = 0.0f;

		L_MatFloat4x4 matRotZ, matRotX;
		static float fTheta = 0.0f;
		fTheta += 0.001f;

		// Rotation Z
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		// Rotation X
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;

		float secondCubeOff = 0.0f;
		int i = 0;
		for (auto& tri : triangleVertices)
		{
			if (i >= 36)
				secondCubeOff = -3.5f;
			if (i >= 72)
				secondCubeOff = 3.5f;

			L_Float4 t = { tri.postion.x + secondCubeOff, tri.postion.y + secondCubeOff, tri.postion.z + secondCubeOff, tri.postion.w };
			L_Float4 triProjected, triTranslated, triRotatedZ, triRotatedZX;

			// Rotate in Z-Axis
			triRotatedZ = Math::Vector4Transform(t, matRotZ);

			// Rotate in X-Axis
			triRotatedZX = Math::Vector4Transform(triRotatedZ, matRotX);

			// Offset into the screen
			triTranslated.x = triRotatedZX.x - 2.0f;
			triTranslated.y = triRotatedZX.y - 2.0f;
			triTranslated.z = triRotatedZX.z + 10.0f;
			triTranslated.w = triRotatedZX.w;

			// Project triangles from 3D --> 2D
			triProjected = Math::Vector4Transform(triTranslated, matProj);

			// Scale into view
			triProjected.x += 0.5f;
			triProjected.y += 0.5f;
			triProjected.x *= 0.5f * 1200.f;
			triProjected.y *= 0.5f * 800.0f;

			tri.postion.x = triProjected.x / 1000.0f;
			tri.postion.y = triProjected.y / 1000.0f;
			tri.postion.z = triProjected.z / 10.0f;
			tri.postion.w = 1.0f;

			// L_LOG(L_INFO, L"GOOD %f %f %f %f", tri[0], tri[1], tri[2], tri[3]);
			++i;
		}
	}
#endif

	// Note: using upload heaps to transfer static data like vert buffers is not 
	// recommended. Every time the GPU needs it, the upload heap will be marshalled 
	// over. Please read up on Default Heap usage. An upload heap is used here for 
	// code simplicity and because there are very few verts to actually transfer.
	auto aa = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto bb = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
	L_THROW_IF_FAILED(LUL_GET_HARDWARE(m_pHardware)->GetDevice()->CreateCommittedResource(
		&aa,
		D3D12_HEAP_FLAG_NONE,
		&bb,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vbv)));

	// Copy the triangle data to the vertex buffer.
	UINT8* pVertexDataBegin;
	CD3DX12_RANGE readRange(0, 0);        // We do not intend to read from this resource on the CPU.
	L_THROW_IF_FAILED(vbv->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	memcpy(pVertexDataBegin, triangleVertices, sizeof(triangleVertices));
	vbv->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	vbview.BufferLocation = vbv->GetGPUVirtualAddress();
	vbview.StrideInBytes = sizeof(Vertex);
	vbview.SizeInBytes = vertexBufferSize;

	m_pCommandList->IASetVertexBuffers(0, 1, &vbview);
	m_pCommandList->DrawInstanced(108, 1, 0, 0);
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::CloseCommandLine()

{    // Indicate that the back buffer will now be used to present.
	auto pb = LUL_GET_SWAPCHAIN(m_pSwapChain)->GetPresentTarget();
	m_pCommandList->ResourceBarrier(1, &pb);

	L_THROW_IF_FAILED(m_pCommandList->Close());

	ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame
	LUL_GET_SWAPCHAIN(m_pSwapChain)->Present();
}

// -----------------------------------------------------------------------------
void LUL_::Graphics::DX12::Commands::Signal(ID3D12Fence* pFence, const uint64_t uValue) const
{
	L_THROW_IF_FAILED(m_pCommandQueue->Signal(pFence, uValue));
}
