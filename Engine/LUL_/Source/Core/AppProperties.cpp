#include "LUL_.h"

#ifdef _WIN32
    #include <shlobj_core.h>
#endif // _WIN32

#include "AppProperties.hpp"

// AppProperties ---------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::AppProperties& LUL_::AppProperties::Get() noexcept
{
    static AppProperties instance;
    return instance;
}

// -----------------------------------------------------------------------------
bool LUL_::AppProperties::DoesExist(const std::wstring& path)
{
    if (path.empty())
        return false;
    if (path.size() >= LUL_PATH)
    {
        L_LOG(Warning, L"Passed path is out of range for LUL_PATH \"%s\"", __FUNCSIG__);
    }

#ifdef _WIN32
    DWORD dwAttrib = GetFileAttributes(path.c_str());
    if (path.back() == L'\\' ||
        path.back() == L'/')
    {
        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
    }
    else
    {
        return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
            (dwAttrib & ~FILE_ATTRIBUTE_DIRECTORY));
    }
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}

// -----------------------------------------------------------------------------
const std::wstring& LUL_::AppProperties::CreatePath(const std::wstring& path)
{
    LUL_PROFILER_TIMER_START();

    if (path.empty())
    {
        return path;
    }
    if (DoesExist(path))
    {
        return path;
    }

    size_t skip = path.size();
    size_t lastSlashIndx;

    if (path.back() == L'\\' || path.back() == L'/')
        skip--;

    lastSlashIndx = path.find_last_of(L'\\', skip);
    if (lastSlashIndx == std::string::npos)
        lastSlashIndx = path.find_last_of(L'/', skip);

    std::filesystem::create_directories(path.substr(0, lastSlashIndx));

    if (path.back() != L'\\' && 
        path.back() != L'/')
    {
        MakeFile(path);
    }
    else
    {
        MakeDir(path);
    }

    return path;
}

// -----------------------------------------------------------------------------
std::wstring LUL_::AppProperties::CreatePathInKnownDir(const KnownDirs& kd, const wchar_t* path)
{
    std::wstring result;

    switch (kd)
    {
    case (Current):
    {
        result = m_CurPath;
        break;
    }
    case (Appdata):
    {
        result = m_AppdataPath;
        break;
    }
    case (CurrentProject):
    {
        result = m_CurPrjPath;
        break;
    }
    default:
    {
        L_LOG(Warning, L"Unkown directory passed fore creating in known dir \"%lS\" | kd = %d", path, kd);
        result = path;
    }
    }
    if (path)
        result += path;

    return AppProperties::CreatePath(result);
}

// -----------------------------------------------------------------------------
int LUL_::AppProperties::AddIWindow(LUL_::IWindow* pW)
{
    m_Windows.push_back(pW);
    return static_cast<int>(m_Windows.size() - 1); // As index
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::RemoveIWindow(LUL_::IWindow* pW)
{
    size_t i = m_Windows.remove(pW);
    if (!i)
        throw Exceptions::ItemNotFound(
            0, 
            static_cast<void*>(pW),
            LUL_EXCPT_HELPER());
    else if (i > 1)
    {
        L_LOG(
            L_WARNING, L"Removed more than one window? | amount = %llu | ptr = %p",
            i,
            pW);
        return;
    }
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::TryQuitApplication()
{
    L_LOG(Info, L"Post shut down on application");

#ifdef _WIN32
    PostQuitMessage(0);
    return;
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::ForceQuitApplication() noexcept
{
    L_LOG(Warning, L"Forcing shut down on application");

    // Set global app ptr to null

    // Flush logger

    exit(0);
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::SetAppName(char const* const n)
{
    if (!LUL_IS_STR_BUF_NULL(m_AppName))
    {
        L_LOG(Warning, L"App name is already set?");
        return;
    }

    if (strcpy_s(m_AppName, n) == ERANGE)
    {
        L_LOG(Warning, L"Passed app name str was outsied of range");
        return;
    }
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::SetAppClass(char const* const cl)
{
    if (!LUL_IS_STR_BUF_NULL(m_AppClass))
    {
        L_LOG(Warning, L"App class is already set?");
        return;
    }

    if (strcpy_s(m_AppClass, cl) == ERANGE)
    {
        L_LOG(Warning, L"Passed app class str was outsied of range");
        return;
    }
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::SetAppVersion(char const* const v)
{
    if (!LUL_IS_STR_BUF_NULL(m_AppVersion))
    {
        L_LOG(Warning, L"App version is already set?");
        return;
    }
    
    if (strcpy_s(m_AppVersion, v) == ERANGE)
    {
        L_LOG(Warning, L"Passed app version str was outsied of range");
        return;
    }
}

// Private ---------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::AppProperties::MakeDir(const std::wstring& path)
{
    try
    {
        std::filesystem::create_directory(path);
    }
    catch (std::exception e)
    {
        throw e;
    }
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::MakeFile(const std::wstring& path)
{
    std::fstream file(path, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        throw LUL_::Exceptions::InvalidArg(LUL_EXCPT_HELPER());
    }

    file.close();
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::FindBootTime() noexcept
{
    time_t tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm localt;
    if (localtime_s(&localt, &tt) == EINVAL)
    {
        m_AppBootTime[0] = L'0';
        return;
    }

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << std::to_string(localt.tm_hour)
        << "-"
        << std::setfill('0') << std::setw(2) << std::to_string(localt.tm_min)
        << "-"
        << std::setfill('0') << std::setw(2) << std::to_string(localt.tm_sec);

    strcpy_s(m_AppBootTime, ss.str().c_str());
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::FindAppdataPath() noexcept
{
#ifdef _WIN32
    LUL_PROFILER_TIMER_START();

    PWSTR appDataPath = nullptr;
    SHGetKnownFolderPath(
        FOLDERID_RoamingAppData,
        KF_FLAG_DEFAULT,
        NULL,
        &appDataPath);

    // Copy path, free the memory, add suffix
    m_AppdataPath = appDataPath;
    CoTaskMemFree(appDataPath);
    m_AppdataPath += L"\\Im-Bee\\";

    MakeDir(m_AppdataPath);

    return;
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::FindCurrentPath() noexcept
{
#ifdef _WIN32
    LUL_PROFILER_TIMER_START();

    LPWSTR tmp = (LPWSTR) CoTaskMemAlloc(sizeof(WCHAR) * LUL_PATH);
    if (!tmp)
    {
        m_CurPath = L"";
        return;
    }

    GetModuleFileName(NULL, tmp, LUL_PATH);
    m_CurPath = tmp;
    CoTaskMemFree(tmp);

    m_CurPath.erase(m_CurPath.find_last_of(L'\\'));

    return;
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::FindCurrentProjectPath() noexcept
{
#ifdef _WIN32
    LUL_PROFILER_TIMER_START();

    m_CurPrjPath = m_CurPath;
    m_CurPrjPath.pop_back();
    m_CurPrjPath = m_CurPrjPath.substr(0, m_CurPrjPath.find_last_of('\\'));
    #ifdef _DEBUG
    m_CurPrjPath = m_CurPrjPath.substr(0, m_CurPrjPath.size() - std::string("Bin64\\Projects\\Debug\\").size()) + L"\\";
    #else
    m_CurPrjPath = m_CurPrjPath.substr(0, m_CurPrjPath.size() - std::string("Bin64\\Projects\\Release\\").size()) + L"\\";
    #endif // _DEBUG


    return;
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}
