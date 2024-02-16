#include "LUL_.h"

void LUL_::SetupLibDirs()
{
#ifdef _WIN32
    DLL_DIRECTORY_COOKIE cookie = 0;
    
    #ifdef _LUL_STANDALONE_PACKAGE
        cookie = AddDllDirectory(AppProperties::Get().CreatePathInKnownDir(KnownDirs::Current, 
            L"\\Dll\\").c_str());
        if (!cookie)
            throw Exceptions::Internal(LUL_EXCPT_HELPER());

        return;
    #else


        return;
    #endif // !_LUL_STANDALONE_PACKAGE
#else
    throw Exceptions::NotImplemented(LUL_EXCPT_HELPER());
#endif // _WIN32
}
