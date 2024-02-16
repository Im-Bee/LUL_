#include "LUL_.h"

LUL_::Exceptions::Exception::Exception(char const* const msg, char const* const file, const int& line) noexcept
    : exception(msg)
{
    if (line <= -1)
        return;

    const char* fileSubPath = strstr(file, "LUL_");
    if (fileSubPath != NULL) 
    {
        L_LOG(L_ERROR, L"Exception thrown in file \"%S\" at line %d", fileSubPath, line);
    }
    else
    {
        L_LOG(L_ERROR, L"Exception thrown in file \"%S\" at line %d", file, line);
    }
}
