#pragma once

namespace LUL_
{
    enum LUL_EXPORT KnownDirs
    {
        Current = 0x10,
        Appdata = 0x20,
        CurrentProject = 0x30
    };

    class LUL_EXPORT AppProperties
    {
    private:

        std::wstring m_AppdataPath, m_CurPath, m_CurPrjPath;

        char m_AppName[LUL_STRING_V_SMALL] = { 0 },
            m_AppClass[LUL_STRING_V_SMALL] = { 0 },
            m_AppBootTime[LUL_STRING_V_SMALL] = { 0 },
            m_AppVersion[LUL_STRING_V_SMALL] = { 0 };

        std::list<LUL_::IWindow*> m_Windows =
            std::list<LUL_::IWindow*>();
        
    private:

        // Singleton -----------------------------------------------------------

        AppProperties() noexcept
        {
            FindBootTime();
            FindAppdataPath();
            FindCurrentPath();
            FindCurrentProjectPath();
        }

    public:

        AppProperties(AppProperties&&) = delete;
        AppProperties(const AppProperties&) = delete;

        static AppProperties& Get() noexcept;

        ~AppProperties() noexcept = default;

    public:

        /*
        * Checks if file/dir exist
        * @param path: If the path ends with '/' or '\\' then checks for dir otherwise
        * checks for file. */
        static bool DoesExist(const std::wstring& path);

        /*
        * Creates provided path. If there are dirs in path that aren't created yet
        * then creates them too.
        * @param path: If the path ends with '/' or '\\' then creates a new dir
        * otherwise creates new file. 
        * @returns The path. */
        static const std::wstring& CreatePath(const std::wstring& path);

        /*
        * Creates provided path in known dir. If there are dirs in path that aren't
        * yet created then creates them too.
        * Can be used to retrive the known dir if no path provided.
        * @param Optional - path: If the path ends with '/' or '\\' then creates a new dir
        * otherwise creates new file.
        * @returns The path. */
        std::wstring CreatePathInKnownDir(
            const KnownDirs& kd, 
            const wchar_t* path = nullptr);

        int AddIWindow(LUL_::IWindow* pW);

        void RemoveIWindow(LUL_::IWindow* pW);
        
        static void TryQuitApplication();

        static void ForceQuitApplication() noexcept;

    public:

        // Getters -------------------------------------------------------------

        char const* const GetAppName() const { return m_AppName; }

        char const* const GetAppClass() const { return m_AppClass; }

        char const* const GetAppBootTime() const { return m_AppBootTime; }

        char const* const GetAppVersion() const { return m_AppVersion; }

    public:

        // Setters -------------------------------------------------------------

        void SetAppName(char const* const n);
        
        void SetAppClass(char const* const cl);
        
        void SetAppVersion(char const* const v);

    private:

        static void MakeDir(const std::wstring& path);

        static void MakeFile(const std::wstring& path);

        void FindBootTime() noexcept;

        void FindAppdataPath() noexcept;

        void FindCurrentPath() noexcept;

        void FindCurrentProjectPath() noexcept;

    };
}