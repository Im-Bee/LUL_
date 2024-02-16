#pragma once

namespace LUL_
{
    class LUL_EXPORT IWindow
    {
    private:

        int m_WindowIndex = 0;

    public:

        IWindow() noexcept;

        ~IWindow() noexcept;

    public:

        const int& GetWindowIndex() const noexcept { return m_WindowIndex; }

    private:

        void AddItselfToAppProperties() noexcept;

        void RemoveItselfFromAppProperties() noexcept;

    };
}