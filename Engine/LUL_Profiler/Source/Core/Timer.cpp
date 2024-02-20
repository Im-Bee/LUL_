#include "LUL_Profiler.h"

#include <cstdint>
#include <chrono>
#include <filesystem>  
#include <fstream>
#include <thread>

// Snapshot --------------------------------------------------------------------
//  ----------------------------------------------------------------------------
LUL_::Profiler::Snapshot::Snapshot(char const* const fnsig, int pid) noexcept
    : m_Data({ fnsig, pid, __threadid() })
{
    m_Data.Start = std::chrono::high_resolution_clock::now();
}

// -----------------------------------------------------------------------------
LUL_::Profiler::Snapshot::~Snapshot() noexcept
{
    m_Data.Stop = std::chrono::high_resolution_clock::now();
    Timer::Get().AddSnapshot(m_Data);
}

// Timer -----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::Profiler::Timer& LUL_::Profiler::Timer::Get() noexcept
{
    static Timer instance;
    return instance;
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Timer::OutputResults()
{
    if (!m_OutputPath.empty())
        WriteToFile(m_OutputPath);
    else
        WriteToFile(CreateResultDir());
}

// Setters ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Profiler::Timer::SetPath(const wchar_t* path)
{
    namespace fs = std::filesystem;

    if (!fs::exists(path))
        return;

    m_OutputPath = path;
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Timer::AddSnapshot(SnapshotData s)
{
    m_Snapshots.push_back(s);
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
const wchar_t* LUL_::Profiler::Timer::CreateResultDir()
{
    namespace fs = std::filesystem;
    
    static const fs::path r("./Results/");

    if (fs::exists(r))
        return r.c_str();

    return fs::create_directory(r) ? r.c_str() : L"";
}

// -----------------------------------------------------------------------------
void LUL_::Profiler::Timer::WriteToJson(std::wofstream& fst, 
    const SnapshotData& s,
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
void LUL_::Profiler::Timer::WriteToFile(std::wstring path)
{
    namespace ch = std::chrono;

    if (m_Snapshots.empty())
        return;
    
    bool wroteAnything = false;
    std::wofstream outFile(path + 
        std::to_wstring(ch::high_resolution_clock::now().time_since_epoch().count()) + 
        L"-Results.json");

    if (!outFile.is_open())
        throw;

    outFile << "{\"otherData\": {},\"traceEvents\":[";
    for (auto& i : m_Snapshots)
    {
        wroteAnything = true;
        
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(
            i.Start).time_since_epoch().count();
        long long stop = std::chrono::time_point_cast<std::chrono::microseconds>(
            i.Stop).time_since_epoch().count();

        long long dur = stop - start;

        WriteToJson(outFile, 
            i,
            dur,
            start);
            
    }
    if (wroteAnything)
        outFile.seekp(-1, std::ios::cur);
    outFile << "]}";

    outFile.close();

    m_Snapshots.clear();
}
