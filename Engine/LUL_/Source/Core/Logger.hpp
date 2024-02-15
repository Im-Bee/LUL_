#pragma once

namespace LUL_
{
    enum LUL_EXPORT Severity
    {
        Info,
        Warning,
        Error
    };

    typedef std::pair<Severity, std::wstring> LogData;

    class LUL_EXPORT Logger
    {
    private:

        std::wstring m_OutFilePath;

    private:

        // Singleton -----------------------------------------------------------

        Logger() noexcept
        {
            CreateOutFile();
            CreateThread();
        }

    public:

        Logger(Logger&&) = delete;
        Logger(const Logger&) = delete;

        static Logger& Get() noexcept
        {
            static Logger instance;
            return instance;
        }
        ~Logger() noexcept
        {
            KillThread();
        }

    public:

        void Log(const Severity& s, wchar_t const* const msg, ...);

    private:

        void CreateOutFile() noexcept;
        
        void CreateThread();

        void ThreadLoop();

        void KillThread();

    private:

        std::atomic_bool m_Thread = std::atomic_bool(false);
        std::thread m_LogThread;
        // std::atomic<std::queue<LogData>> m_ThreadQueue = std::atomic<std::queue<LogData>>();
    };
}