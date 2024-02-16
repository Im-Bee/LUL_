#include "LUL_.h"

#include <cstdarg>

// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::Logger::Log(const Severity s, wchar_t const* const msg, ...)
{
    using namespace std::chrono_literals;

    wchar_t fmtBuff[LUL_STRING_BIG] = { 0 };

    va_list args;
    va_start(args, msg);
    vswprintf_s(fmtBuff, msg, args);
    va_end(args);

#if LUL_CORE_MULTITHREADED
    while (m_QueueLock.load())
    {
        std::this_thread::sleep_for(1ns);
    }

    m_QueueLock.store(true);
    m_ThreadQueue.push({ s, std::wstring(fmtBuff), std::chrono::system_clock::now() });
    m_QueueLock.store(false);
#else
    std::wstring str(fmtBuff);
    TagFmt(s, str);
    if (!LogFmtMsg(str))
    {
#ifdef _DEBUG
        throw Exceptions::Internal(LUL_EXCPT_HELPER());
#else
        retrun;
#endif // _DEBUG
    }
#endif // LUL_CORE_MULTITHREADED
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
std::wstring LUL_::Logger::CreateOutFile()
{
    LUL_PROFILER_TIMER_START();

    std::wstringstream fileNamePath;

    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm localt;
    if (localtime_s(&localt, &tt) == EINVAL)
    {
        throw Exceptions::Internal(LUL_EXCPT_HELPER());
    }

    fileNamePath << std::to_wstring(localt.tm_year + 1900)
        << L"-"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_mon + 1)
        << L"-"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_mday)
        << L"\\"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_hour)
        << L"-"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_min)
        << L"-"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_sec)
        << L".log";

    return fileNamePath.str();
}

// -----------------------------------------------------------------------------
void LUL_::Logger::CreateOutPath() noexcept
{
    LUL_PROFILER_TIMER_START();
    try
    {
        m_OutFilePath = AppProperties::Get().CreatePathInKnownDir(KnownDirs::Appdata,
            std::wstring(LUL_LOG_DIR) + CreateOutFile());
    }
    catch (std::exception e)
    {
        m_OutFilePath = LUL_LOG_PATH_IF_ERROR;
        Log(Warning, L"%S", e.what());
    }
    catch (...)
    {
        m_OutFilePath = LUL_LOG_PATH_IF_ERROR;
        Log(Warning, L"Unknown error");
    }
}

// -----------------------------------------------------------------------------
void LUL_::Logger::CreateThread() noexcept
{
    LUL_PROFILER_TIMER_START();

#if LUL_CORE_MULTITHREADED
    m_Thread.store(true);
    m_LogThread = std::thread(&LUL_::Logger::ThreadLoop, this);
#else
    return;
#endif // LUL_CORE_MULTITHREADED
}

// -----------------------------------------------------------------------------
void LUL_::Logger::PrintHeader() noexcept
{
    LUL_PROFILER_TIMER_START();

    Log(L_PLAIN,
        L"----------------------------------------------------------------\n\
Application: %S\n\
Boot time: %S\n\
Version: %S\n\
----------------------------------------------------------------",
LUL_::AppProperties::Get().GetAppName(),
LUL_::AppProperties::Get().GetAppBootTime(),
LUL_::AppProperties::Get().GetAppVersion());
}

// -----------------------------------------------------------------------------
void LUL_::Logger::PrintFooter() noexcept
{
    Log(L_PLAIN,
        L"----------------------------------------------------------------\n\
Im-Bee 2024\n\
----------------------------------------------------------------");
}

// -----------------------------------------------------------------------------
void LUL_::Logger::TagFmt(const Severity& s,
    std::wstring& msg,
    const std::chrono::time_point<std::chrono::system_clock>& time)
{
    static bool failed = false;

    time_t tt = std::chrono::system_clock::to_time_t(time);
    tm localt;
    if (localtime_s(&localt, &tt) == EINVAL &&
        !failed)
    {
        failed = true;

        L_LOG(Error, L"Couldn't convert to localtime_s");
#ifdef _DEBUG
        throw Exceptions::Internal(LUL_EXCPT_HELPER());
#endif // _DEBUG
    }

    std::wstringstream raw;

    raw << L"["
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_hour)
        << L":"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_min)
        << L":"
        << std::setfill(L'0') << std::setw(2) << std::to_wstring(localt.tm_sec)
        << L"]";

    switch (s)
    {
    case (Plain):
    {
        return;
    }
    case (Info):
    {
        raw << " [Info] "
            << msg;

        break;
    }
    case (Warning):
    {
        raw << L" [Warning] "
            << msg;

        break;
    }
    case (Error):
    {
        raw << L" [Error] "
            << msg;

        break;
    }
    default:
    {
        raw << L" [Unknown] "
            << msg;

        break;
    }
    }

    msg = raw.str();
}

// -----------------------------------------------------------------------------
bool LUL_::Logger::LogFmtMsg(std::wstring& msg)
{
    using namespace std::chrono_literals;

    static bool redo = false;
    static int retries = 0;

    std::wofstream out(m_OutFilePath, std::ios::out | std::ios::app);
    if (!out.is_open())
    {
        redo = true;
        retries++;

        if (retries >= LUL_LOG_RETRIES_AMOUNT)
        {
#ifdef _DEBUG
            throw Exceptions::Internal(LUL_EXCPT_HELPER());
#else
            return false;
#endif // _DEBUG
        }

        std::this_thread::sleep_for(1ns);
        return LogFmtMsg(msg);
    }

    if (redo)
    {
        static std::wstring errorMsg = L"The following string couldn't be logged through " +
            std::to_wstring(retries) + L"retries \"" + msg + L'\"';
        TagFmt(Error, errorMsg);

        out << errorMsg << L'\n';

        out.close();
        redo = false;
        retries = 0;
        return true;
    }

    out << msg << L'\n';
    out.close();
    return true;
}

// -----------------------------------------------------------------------------
void LUL_::Logger::ThreadLoop()
{
    using namespace std::chrono_literals;
    static const auto timeout = 5ms;

    while (m_Thread.load())
    {
        std::this_thread::sleep_for(timeout);

        if (m_ThreadQueue.empty())
        {
            m_QueueLock.store(false);
            continue;
        }

        auto& i = m_ThreadQueue.front();

        TagFmt(std::get<0>(i), std::get<1>(i), std::get<2>(i));
        LogFmtMsg(std::get<1>(i));

        if (m_QueueLock.load())
        {
            std::this_thread::sleep_for(timeout);
        }

        m_QueueLock.store(true);
        m_ThreadQueue.pop();
        m_QueueLock.store(false);
    }
}

// -----------------------------------------------------------------------------
void LUL_::Logger::KillThread() noexcept
{
#if LUL_CORE_MULTITHREADED
    m_Thread.store(false);
    m_LogThread.join();
#else
    return;
#endif // LUL_CORE_MULTITHREADED
}

// -----------------------------------------------------------------------------
void LUL_::Logger::DumpQueue() noexcept
{
    while (!m_ThreadQueue.empty())
    {
        auto& i = m_ThreadQueue.front();

        TagFmt(std::get<0>(i), std::get<1>(i), std::get<2>(i));
        LogFmtMsg(std::get<1>(i));

        m_ThreadQueue.pop();
    }
}
