#pragma once

#include "LUL_.h"

#if defined(_LUL_DX12) && defined(_WIN64)
	#pragma region System dependent libs
		#ifdef _DEBUG
			#define D3DCOMPILE_DEBUG 1
		#endif // _DEBUG

		#pragma warning ( push )
		#pragma warning (disable : 4005)
		#include <wrl.h>
		#include <d3d12.h>
		#pragma comment(lib, "d3d12")
		#include <dxgi1_6.h>
		#pragma comment(lib, "dxgi")
		#include <d2d1.h>
		#pragma comment(lib, "d2d1")
		#include <dwrite.h>
		#pragma comment(lib, "Dwrite")
		#include <D3Dcompiler.h>
		#pragma comment(lib, "D3Dcompiler")
		#include <DirectXMath.h>
		#include <d3d11on12.h>
		#include <dxgidebug.h>
		#pragma comment(lib, "D3D11")
		#pragma comment(lib, "dxguid")
		#pragma comment(lib, "kernel32")
		#pragma warning ( pop )
	#pragma endregion 

	#pragma region Engine macros
		#define L_THROW_IF_FAILED(hr)											\
					if (FAILED(hr)) throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER())

		#define LUL_GET_HARDWARE(hardware) static_cast<const LUL_::Graphics::DX12::Hardware*>(hardware.get())
		#define LUL_GET_SWAPCHAIN(swapChain) static_cast<const LUL_::Graphics::DX12::SwapChain*>(swapChain.get())
		#define LUL_GET_MEMORY(memory) static_cast<const LUL_::Graphics::DX12::Memory*>(memory.get())
		#define LUL_GET_COMMANDS(commands) static_cast<const LUL_::Graphics::DX12::Commands*>(commands.get())

		#ifdef _DEBUG
			#define LUL_DX_LOG_CREATE() L_LOG(L_INFO, L"Create %S | %p", __func__, this)

			#define LUL_SET_DX_NAME(obj, name)												\
							const wchar_t* theName = L#name;												\
							if (FAILED(obj->SetPrivateData(													\
								WKPDID_D3DDebugObjectName,													\
								sizeof(theName),															\
								theName)))																	\
							{																				\
								L_LOG(L_WARNING, L"Couldn't set the name for %p to %lS", obj, theName);		\
							}

			#define LUL_SET_DX12_NAME(obj, name) obj->SetName(L#name)
		#else
			#define LUL_DX_LOG_CREATE()
			#define LUL_SET_DX_NAME(obj, name)
			#define LUL_SET_DX12_NAME(obj, name)
		#endif // _DEBUG
	#pragma endregion

	#pragma region Engine includes
		#pragma warning ( push )
		// Warning C4251 Class needs to have dll - interface to be used by clients of class.
		// Warning C4275 Non dll-interface class used as base.
		#pragma warning (disable : 4251 4275)
			#include "Interfaces/IRenderer.hpp"
			
			#include "Graphics/DirectX12/d3dx12.h"
			#include "Graphics/DirectX12/Renderer.hpp"
		#pragma warning ( pop )
	#pragma endregion
#endif

