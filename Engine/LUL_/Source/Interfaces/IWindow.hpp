#pragma once

namespace LUL_
{
    class LUL_EXPORT IWindow
    {
    private:    

        int m_WindowIndex = LUL_WINDOW_UNITIALIZED_INDEX;
        
        wchar_t m_WindowName[LUL_STRING_V_SMALL],
            m_WindowClass[LUL_STRING_V_SMALL];

        L_Vec2<uint32_t> m_WindowDimensions = { 1200, 800 };

    public:

        IWindow() = delete;
        IWindow(wchar_t const* const windowName,
            wchar_t const* const windowClass) noexcept;

        ~IWindow() noexcept;

    public:

        // Getters ---------------------------------------------------------------------

        const int& GetWindowIndex() const noexcept { return m_WindowIndex; }

        wchar_t const* const GetWindowName() const noexcept { return m_WindowName; }
        
        wchar_t const* const GetWindowClass() const noexcept { return m_WindowClass; }

        L_Vec2<uint32_t>& GetWindowDimensions() { return m_WindowDimensions; }

    protected:

        // Setters ---------------------------------------------------------------------

        void SetWindowDimensions(L_Vec2<uint32_t> d) { m_WindowDimensions = d; }

    private:

        void AddItselfToAppProperties() noexcept;

        void RemoveItselfFromAppProperties() noexcept;

    };
}