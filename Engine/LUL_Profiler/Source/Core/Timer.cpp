#include "LUL_Profiler.h"

#include <cstdint>
#include <chrono>
#include <filesystem>  
#include <fstream>
#include <thread>

// Timer -----------------------------------------------------------------------

// Public ----------------------------------------------------------------------

// -----------------------------------------------------------------------------
void LUL_::Profiler::Detail::Timer::Start(LUL_::Profiler::Detail::Snapshot s)
{
    m_StartSnapshots.push_back(s);
    m_StartSnapshots.back().Point = std::chrono::high_resolution_clock::now();
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Detail::Timer::Stop(LUL_::Profiler::Detail::Snapshot s)
{
    m_StopSnapshots.push_back(s);
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Detail::Timer::OutputResults()
{
    if (!m_OutputPath.empty())
        WriteToFile(m_OutputPath);
    else
        WriteToFile(CreateResultDir());
}

// Setters ---------------------------------------------------------------------

// -----------------------------------------------------------------------------
void LUL_::Profiler::Detail::Timer::SetPath(const wchar_t* path)
{
    namespace fs = std::filesystem;

    if (!fs::exists(path))
        return;

    m_OutputPath = path;
}

// Private ---------------------------------------------------------------------

// -----------------------------------------------------------------------------
const wchar_t* LUL_::Profiler::Detail::Timer::CreateResultDir()
{
    namespace fs = std::filesystem;
    
    static const fs::path r("./Results/");

    if (fs::exists(r))
        return r.c_str();

    return fs::create_directory(r) ? r.c_str() : L"";
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Detail::Timer::WriteToJson(std::wofstream& fst, 
    const Snapshot& s, 
    const long long& dur, 
    const long long& start)
{
    fst << "{";
    fst << "\"cat\":\"function\",";
    fst << "\"dur\":" << dur << ',';
    fst << "\"name\":\"" << s.Name << "\",";
    fst << "\"ph\":\"X\",";
    fst << "\"pid\":" << s.PID << ",";
    fst << "\"tid\":" << s.TID << ",";
    fst << "\"ts\":" << start;
    fst << "},";
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Detail::Timer::WriteToFile(std::wstring path)
{
    namespace ch = std::chrono;

    if (m_StartSnapshots.empty())
        return;
    
    bool wrote = false;
    // In cases where LUL_PROFILER_TIMER_STOP wasn't called
    const long long artificialStop = ch::time_point_cast<ch::microseconds>(
        ch::high_resolution_clock::now()).time_since_epoch().count();
    std::wofstream outFile(path + 
        std::to_wstring(ch::high_resolution_clock::now().time_since_epoch().count()) + 
        L"-Results.json");

    if (!outFile.is_open())
        throw;

    outFile << "{\"otherData\": {},\"traceEvents\":[";
    for (size_t i = 0; i < m_StartSnapshots.size(); i++)
    {
        // Reset value
        wrote = false;
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(
            m_StartSnapshots[i].Point).time_since_epoch().count();

        for (size_t k = 0; k < m_StopSnapshots.size(); k++)
        {
            if (m_StartSnapshots[i].PID != m_StopSnapshots[k].PID ||
                strcmp(m_StartSnapshots[i].Name, m_StopSnapshots[k].Name) != 0 ||
                strcmp(m_StartSnapshots[i].File, m_StopSnapshots[k].File) != 0)
                continue;

            long long stop = std::chrono::time_point_cast<std::chrono::microseconds>(
                m_StopSnapshots[k].Point).time_since_epoch().count();
            long long dur = stop - start;
            if (dur < 0)
                continue;

            WriteToJson(outFile, 
                m_StartSnapshots[i],
                dur,
                start);
            
            wrote = true;

            // Go to the next StartSnapshot
            break;
        }
        
         // Try to write it with artificialStop as stop point
        if (!wrote)
        {
            wrote = true;
            WriteToJson(outFile, 
                m_StartSnapshots[i],
                artificialStop - start,
                start);
        }
    }
    if (wrote)
        outFile.seekp(-1, std::ios::cur);
    outFile << "]}";

    outFile.close();

    m_StartSnapshots.clear();
    m_StopSnapshots.clear();
}

// LUL_::Profiler --------------------------------------------------------------

// -----------------------------------------------------------------------------
void LUL_::Profiler::SetTimerResultDir(const wchar_t* path)
{
    LUL_::Profiler::Detail::Timer::Get().SetPath(path);
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::StartTimer(char const* const fnsig, int pid)
{
    LUL_::Profiler::Detail::Timer::Get().Start({ fnsig, __FILE__, pid, __threadid() });
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::StopTimer(char const* const fnsig, int pid)
{
    auto p = std::chrono::high_resolution_clock::now();
    LUL_::Profiler::Detail::Timer::Get().Stop({ fnsig, __FILE__, pid, __threadid(), p });
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::OutputTimerResults()
{
    LUL_::Profiler::Detail::Timer::Get().OutputResults();
}