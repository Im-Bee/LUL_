#pragma once

namespace LUL_
{
    class LUL_EXPORT EmptyWindow
        : public LUL_::IWindow
    {
    protected:

#ifdef _WIN32
        HWND m_HWND = nullptr;
#endif // _WIN32

    public:

        EmptyWindow();
        EmptyWindow(
            wchar_t const* const windowName,
            wchar_t const* const windowClass);

        ~EmptyWindow() = default;

    public:

        void Show() override;

        void Close() override;

    public:

        // Getters ---------------------------------------------------------------------

#ifdef _WIN32
        virtual void* GetHandle() const noexcept override { return m_HWND; }
#endif // _WIN32

    public:

        // Setters ---------------------------------------------------------------------

        void SetDimensions(L_Vec2<uint32_t> d);

        void SetDimensions(uint32_t width, uint32_t height)
        {
            return SetDimensions({ width, height });
        }

    protected:

#ifdef _WIN32
        virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif // _WIN32

    private:

        bool Create();

#ifdef _WIN32
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        
        LRESULT OnResizeMessage(LPARAM lParam);

        LRESULT OnDestroyMessage();
#endif // _WIN32
    };
}