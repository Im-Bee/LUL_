#pragma once

#define LUL_LOG_DIR L"\\Logs\\"
#define LUL_LOG_PATH_IF_ERROR L".\\Error.log"
#define LUL_LOG_RETRIES_AMOUNT 100

#define L_PLAIN   LUL_::Severity::Plain
#define L_INFO    LUL_::Severity::Info
#define L_WARNING LUL_::Severity::Warning
#define L_ERROR   LUL_::Severity::Error

namespace LUL_
{
    enum LUL_EXPORT Severity
    {
        Plain = 0x00,
        Info = 0x10,
        Warning = 0x20,
        Error = 0x30
    };

    typedef std::tuple<Severity, 
        std::wstring,
        std::chrono::time_point<std::chrono::system_clock>> LogData;

    class LUL_EXPORT Logger
    {
    private:

        std::wstring m_OutFilePath;

    private:

        // Singleton -----------------------------------------------------------

        Logger() noexcept
        {
            CreateThread();
            CreateOutPath();
            PrintHeader();
        }

    public:

        Logger(Logger&&) = delete;
        Logger(const Logger&) = delete;

        static Logger& Get() noexcept;

        ~Logger() noexcept
        {
            KillThread();
            PrintFooter();
            DumpQueue();
        }

    public:

        void Log(const Severity s, wchar_t const* const msg, ...);

        void WaitForTraffic();

    private:

        std::wstring CreateOutFile();
        
        void CreateOutPath() noexcept;

        void CreateThread() noexcept;

        void PrintHeader() noexcept;

        void PrintFooter() noexcept;

        void TagFmt(
            const Severity& s, 
            std::wstring& msg, 
            const std::chrono::time_point<std::chrono::system_clock>& time = std::chrono::system_clock::now());

        bool LogFmtMsg(std::wstring& msg);

        void ThreadLoop();

        void KillThread() noexcept;

        void DumpQueue() noexcept;

    private:

        std::atomic_bool m_Thread = std::atomic_bool(false);
        std::thread m_LogThread = std::thread();
        std::atomic_bool m_QueueLock = std::atomic_bool(false);
        std::queue<LogData> m_ThreadQueue = std::queue<LogData>();
    };
}