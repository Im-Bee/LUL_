#pragma once

namespace LUL_
{
    class LUL_EXPORT Logger
    {
    private:

        std::wstring m_OutFilePath;

    private:

        // Singleton -----------------------------------------------------------

        Logger() noexcept
        {
            CreateOutFile();
        }

    public:

        Logger(Logger&&) = delete;
        Logger(const Logger&) = delete;

        static Logger& Get() noexcept
        {
            static Logger instance;
            return instance;
        }
        ~Logger() noexcept = default;

    public:

        void Log();

    private:

        void CreateOutFile() noexcept;

    };
}