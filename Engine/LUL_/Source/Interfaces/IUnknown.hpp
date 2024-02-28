#pragma once

#define LUL_IUNKNOWN_IMPL(className)                                                \
public:                                                                             \
    char const* const GetClass() const override { return #className; }              \
    static char const* const GetClassId() { return #className; }                    \
private:                                                        

namespace LUL_
{
    class LUL_EXPORT IUnknown
    {
    public:

        IUnknown() = default;

        ~IUnknown() = default;

    public:

        virtual char const* const GetClass() const { return "IUnknown"; }

        bool operator==(const IUnknown& other) const;

        bool operator!=(const IUnknown& other) const;

    };
}