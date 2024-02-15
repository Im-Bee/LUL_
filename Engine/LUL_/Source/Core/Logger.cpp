#include "LUL_.h"

// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Logger::Log(const Severity& s, wchar_t const* const msg, ...)
{
#if LUL_CORE_MULTITHREADED
    
#else

#endif // LUL_CORE_MULTITHREADED
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Logger::CreateOutFile() noexcept
{
    LUL_PROFILER_TIMER_START();

    try
    {
        m_OutFilePath = AppProperties::Get().CreatePathInKnownDir(KnownDirs::Appdata, L"\\Logs\\f.log");
    }
    catch (std::exception e)
    {
        m_OutFilePath = L"ERROR.log";
        L_LOG(Warning, L"%s", e.what());
    }
}

// -----------------------------------------------------------------------------
void LUL_::Logger::CreateThread()
{
#if LUL_CORE_MULTITHREADED

#else
    return;
#endif // LUL_CORE_MULTITHREADED
}

// -----------------------------------------------------------------------------
void LUL_::Logger::ThreadLoop()
{

}

// -----------------------------------------------------------------------------
void LUL_::Logger::KillThread()
{
#if LUL_CORE_MULTITHREADED

#else
    return;
#endif // LUL_CORE_MULTITHREADED
}
