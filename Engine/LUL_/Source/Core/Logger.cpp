#include "LUL_.h"

// Public ----------------------------------------------------------------------

void LUL_::Logger::Log()
{
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
    }

    LUL_PROFILER_TIMER_STOP();
}
