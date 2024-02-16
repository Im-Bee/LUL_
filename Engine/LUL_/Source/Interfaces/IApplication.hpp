#pragma once

#define LUL_DECLARE_APPLICATION(appliactionClass) LUL_::IApplication* LUL_::EntryApplication() { return new appliactionClass; }

namespace LUL_
{
    class LUL_EXPORT IApplication
    {
    public:

        IApplication() = default;

        ~IApplication() = default;

    public:

        virtual void Initialize() = 0;

        virtual void Update() = 0;

        virtual void Destroy() = 0;

    };

    IApplication* EntryApplication();
}   