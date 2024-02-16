#include "LUL_.h"

#ifdef _WIN32
    #include <shlobj_core.h>
#endif // _WIN32

#include "AppProperties.hpp"

// -----------------------------------------------------------------------------
bool LUL_::AppProperties::DoesExist(const std::wstring& path)
{
    if (path.empty())
        return false;
    if (path.size() >= LUL_PATH)
    {
        L_LOG(Warning, L"Passed path is out of range for LUL_PATH for checking if path exists \"%s\"", __FUNCSIG__);
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
std::wstring LUL_::AppProperties::CreatePathInKnownDir(const KnownDirs& kd, const std::wstring& path)
{
    std::wstring result;

    switch (kd)
    {
    case (Current):
    {
        result = m_CurPath + path;
        break;
    }
    case (Appdata):
    {
        result = m_AppdataPath + path;
        break;
    }
    case (CurrentProject):
    {
        result = m_CurPrjPath + path;
        break;
    }
    default:
    {
        L_LOG(Warning, L"Unkown directory passed fore creating in known dir \"%lS\" | kd = %d", path.c_str(), kd);
        result = path;
    }
    }

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
    if (i)
        throw Exceptions::ItemNotFound(0, 
            static_cast<void*>(pW),
            LUL_EXCPT_HELPER());
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::TryQuitApplication()
{
    L_LOG(Info, L"Shuting down application");

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
bool LUL_::AppProperties::MakeDir(const std::wstring& path)
{
    try
    {
        return std::filesystem::create_directory(path);
    }
    catch (std::exception e)
    {
        L_LOG(L_WARNING, L"%S", e.what());
        return false;
    }
}

// -----------------------------------------------------------------------------
bool LUL_::AppProperties::MakeFile(const std::wstring& path)
{
    std::fstream file(path, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        L_LOG(Warning, L"Couldn't create file \"%lS\"", path.c_str());
        return false;
    }

    file.close();
    return true;
}

// -----------------------------------------------------------------------------
void LUL_::AppProperties::FindAppdataPath() noexcept
{
#ifdef _WIN32
    LUL_PROFILER_TIMER_START();

    PWSTR appDataPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData,
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
    m_CurPrjPath = m_CurPrjPath.substr(0, m_CurPrjPath.size() - std::string("Bin64\\Projects\\Debug\\").size()) + L"\\";

    return;
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}
