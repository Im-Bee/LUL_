#ifndef LUL_H
    #define LUL_H

    #pragma region Standard libs
        #ifdef __cplusplus
            #include <chrono>
            #include <exception>
            #include <filesystem>
            #include <fstream>
            #include <future>
            #include <memory>
            #include <string>
            #include <thread>
        #else // C -------------------------------------------------------------
            #include <stdio.h>
        #endif // __cplusplus
    #pragma endregion 

    #pragma region System dependent libs
        #ifdef _WIN32
            #ifndef WIN32_LEAN_AND_MEAN 
                #define WIN32_LEAN_AND_MEAN 
            #endif // !WIN32_LEAN_AND_MEAN 

        #include <Windows.h>
        #endif // !_WIN32
    #pragma endregion 

    #pragma region Engine macros
        #ifdef _LUL_EXPORTS
            #define LUL_EXPORT __declspec(dllexport)
        #else
            #define LUL_EXPORT __declspec(dllimport)
        #endif // !_LUL_EXPORTS

        #define LUL_STRING_V_SMALL  32
        #define LUL_STRING_SMALL    128
        #define LUL_STRING_MEDIUM   256
        #define LUL_STRING_BIG      512
        #define LUL_STRING_V_BIG    1024

        #define LUL_IS_STR_BUF_NULL(str) (str[0] == 0)

        #define LUL_PATH 260

        #define LUL_MAIN_WINDOW_INDEX -1
        #define LUL_UNINITIALIZED_WINDOW_INDEX -2

        #define LUL_EXCPT_HELPER() __FILE__, __LINE__

        // Unit conversion  ----------------------------------------------------
        #define SECONDS_TO_MILLISECONDS(x) (x * 1000)
        #define SECONDS_TO_NANOSECONDS(x) (x * 1000000000)

        #define MILLISECONDS_TO_SECONDS(x) (x / 1000)
        #define MILLISECONDS_TO_NANOSECONDS(x) (x * 1000000)

        #define NANOSECONDS_TO_SECONDS(x) (x / 1000000000)
        #define NANOSECONDS_TO_MILLISECONDS(x) (x / 1000000)

        #define MEM_KiB(x)   (x * 1024)
        #define MEM_MiB(x)   MEM_KiB(x * 1024)
        #define MEM_GiB(x)   MEM_MiB((uint64_t)x * 1024)

        #define MEM_BYTES_TO_KiB(x) (x / 1024)
        #define MEM_BYTES_TO_MiB(x) MEM_BYTES_TO_KiB(x / 1024)
        #define MEM_BYTES_TO_GiB(x) MEM_BYTES_TO_MiB(x / 1024)

        /*
        * Should LUL_ use multithreaded
        * versions of core components if they are available */
        #ifndef _LUL_SINGLE_THREADED_APP
            #define LUL_CORE_MULTITHREADED true
        #else
            #define LUL_CORE_MULTITHREADED false
        #endif // !_SINGLE_THREADED_APP

        #ifdef __cplusplus
            #define LUL_INITIALIZE(...)   LUL::AppProperties::Get().InitCoreLULComponents(__VA_ARGS__);
        #else // C -------------------------------------------------------------
        #endif // __cplusplus

        #ifdef _WIN32
            #define	HInstance() GetModuleHandle(NULL)
        #endif // _WIN32
    #pragma endregion

    #pragma region Engine includes
#include "LUL_.h"
        #include "LUL_Profiler.h"

        #ifdef __cplusplus
            #pragma warning ( push )
            // Warning C4251 Class needs to have dll - interface to be used by clients of class.
            // Warning C4275 Non dll-interface class used as base.
            #pragma warning ( disable : 4251 4275)
            #include "Exceptions/LUL_Exceptions.hpp"        
    
            #include "Core/AppProperties.hpp"    
            #include "Core/Logger.hpp"
            #pragma warning ( pop )
        #else // C -------------------------------------------------------------
        #endif // __cplusplus
    #pragma endregion

    #pragma region Engine loading
        #ifdef __cplusplus
            #include "Core/CoreLoading.hpp"
        #else // C -------------------------------------------------------------
        #endif // __cplusplus
    #pragma endregion

#endif // !LUL_H
