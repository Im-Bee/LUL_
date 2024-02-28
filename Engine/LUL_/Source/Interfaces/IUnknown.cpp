#include "LUL_.h"

bool LUL_::IUnknown::operator==(const IUnknown& other) const
{
    if (!strcmp(this->GetClass(), "IUnknown") &&
        !strcmp(other.GetClass(), "IUnknown"))
    {
        L_LOG(
            L_WARNING, 
            L"IUnknown objects have the same tags, but they are unknown | P = %p P2 = %p",
            this,
            other);

        return true;
    }
    else if (!strcmp(this->GetClass(), other.GetClass()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool LUL_::IUnknown::operator!=(const IUnknown& other) const
{
    if (strcmp(this->GetClass(), other.GetClass()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
