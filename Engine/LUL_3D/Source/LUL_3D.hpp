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

	#pragma region Engine helper functions
		namespace LUL_
		{
			LUL_EXPORT inline const LUL_::IUnknown* GetHelper(const std::shared_ptr<const LUL_::IUnknown>& obj, char const* const original)
			{
			#ifdef _DEBUG
				if (strcmp(obj->GetClass(), original))
					throw LUL_::Exceptions::InvalidArg(LUL_EXCPT_HELPER());
			#endif // _DEBUG
				return obj.get();
			}
		}
	#pragma endregion

	#pragma region Engine macros
		#define L_THROW_IF_FAILED(hr)											\
		if (FAILED(hr)) throw LUL_::Exceptions::Internal(LUL_EXCPT_HELPER())

		#define LUL_GET_HELPER(obj, original) static_cast<const original*>(LUL_::GetHelper(obj, original::GetClassId()))

		#define LUL_GET_HARDWARE(hardwareSharedPtr)	LUL_GET_HELPER(hardwareSharedPtr, LUL_::Graphics::DX12::Hardware)
		#define LUL_GET_SWAPCHAIN(swapChainSharedPtr) LUL_GET_HELPER(swapChainSharedPtr, LUL_::Graphics::DX12::SwapChain)
		#define LUL_GET_MEMORY(memorySharedPtr) LUL_GET_HELPER(memorySharedPtr, LUL_::Graphics::DX12::Memory)
		#define LUL_GET_COMMANDS(commandsSharedPtr) LUL_GET_HELPER(commandsSharedPtr, LUL_::Graphics::DX12::Commands)

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
			#include "Math/Matrices.hpp"

			#include "Interfaces/IRenderer.hpp"
			
			#include "Graphics/DirectX12/d3dx12.h"
			#include "Graphics/DirectX12/Renderer.hpp"
		#pragma warning ( pop )
	#pragma endregion
#endif

