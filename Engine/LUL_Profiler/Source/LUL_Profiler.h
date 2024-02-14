#ifndef LUL_PROFILER_H
    #define LUL_PROFILER_H
    
    #include <process.h>

    #ifdef __cplusplus 
        #include <chrono>
        #include <vector>
        
        #ifdef _LUL_EXPORTS
            #define LUL_EXPORT __declspec(dllexport)
        #else
                #define LUL_EXPORT __declspec(dllimport)
        #endif // !_LUL_EXPORTS

        #if 1
            #define LUL_PROFILER_TIMER_SET_OUTPUT_DIR(path) LUL_::Profiler::SetTimerResultDir(path)

            #define LUL_PROFILER_TIMER_START() LUL_::Profiler::StartTimer(__FUNCSIG__, _getpid())
            #define LUL_PROFILER_TIMER_STOP() LUL_::Profiler::StopTimer(__FUNCSIG__, _getpid())

            /*
            * @param path: Optional */
            #define LUL_PROFILER_TIMER_RESULTS() LUL_::Profiler::OutputTimerResults()
        #else
            #define LUL_PROFILER_TIMER_SET_OUTPUT_DIR(path) 
            #define LUL_PROFILER_TIMER_START() 
            #define LUL_PROFILER_TIMER_STOP()
            #define LUL_PROFILER_TIMER_RESULTS()
        #endif // LUL_PROFILING

        #pragma warning(push)
        // - 4251  - Needs to have dll - interface to be used by clients of class.
        // - 26495 - Variable is uninitialized. Always initialize a member variable(type.6).
        #pragma warning(disable : 4251 26495)

        namespace LUL_::Profiler::Detail
        {
            struct Snapshot
            {
                char const* const Name;
                char const* const File;
                int PID;
                unsigned long TID;
                std::chrono::time_point<std::chrono::steady_clock> Point;
            };

            class Timer
            {
            private:

                std::vector<Snapshot> m_StartSnapshots = std::vector<Snapshot>();
                std::vector<Snapshot> m_StopSnapshots = std::vector<Snapshot>();
                std::wstring m_OutputPath = L"";

            private:

                // Singleton  --------------------------------------------------

                Timer() = default;

            public:

                Timer(Timer&&) = delete;
                Timer(const Timer&) = delete;

                static Timer& Get() noexcept
                {
                    static Timer instance;
                    return instance;
                }
                ~Timer() noexcept = default;

            public:

                void Start(Snapshot s);

                void Stop(Snapshot s);

                void OutputResults();

            public:

                // Setters ---------------------------------------------------------------------

                void SetPath(const wchar_t* path);

            private:

                /*
                * @returns Path to directory with results */
                static const wchar_t* CreateResultDir();

                void WriteToJson(std::wofstream& fst,
                    const Snapshot& s,
                    const long long& dur,
                    const long long& start);
                
                void WriteToFile(std::wstring path);
            };
        }

        namespace LUL_::Profiler
        {
            LUL_EXPORT inline void SetTimerResultDir(const wchar_t* path);

            LUL_EXPORT inline void StartTimer(char const* const fnsig, int pid);

            LUL_EXPORT inline void StopTimer(char const* const fnsig, int pid);

            LUL_EXPORT inline void OutputTimerResults();
        }

        #pragma warning(pop)
    #else // C -----------------------------------------------------------------
    #endif // __cplusplus
#endif // !LUL_PROFILER_H
