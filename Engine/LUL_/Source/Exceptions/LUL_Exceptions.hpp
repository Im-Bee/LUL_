#pragma once



namespace LUL_::Exceptions
{
    class LUL_EXPORT Exception 
        : public std::exception
    {
    public:

        Exception() noexcept = default;
        
        explicit Exception(
            char const* const msg,
            char const* const file = 0,
            const int& line = -1) noexcept;

        ~Exception() noexcept = default;

    };

    class LUL_EXPORT NotImplemented
        : public LUL_::Exceptions::Exception
    {
    public:

        NotImplemented(
            char const* const file = 0,
            const int& line = -1) noexcept
            : Exception("This function isn't implemented on current system", 
                file, 
                line)
        {}

        ~NotImplemented() noexcept = default;

    };

    class LUL_EXPORT InvalidArg
        : public LUL_::Exceptions::Exception
    {
    public:

        InvalidArg(
            char const* const file = 0,
            const int& line = -1) noexcept
            : Exception("Argument passed to this function was invalid",
                file,
                line)
        {}

        ~InvalidArg() noexcept = default;

    };

    class LUL_EXPORT Internal
        : public LUL_::Exceptions::Exception
    {
    public:

        Internal(
            char const* const file = 0,
            const int& line = -1) noexcept
            : Exception("LUL_ coulnd't handle the heat and failed somewhere along the way", 
                file, 
                line)
        {}

        ~Internal() noexcept = default;

    };


    class LUL_EXPORT ItemNotFound
        : public LUL_::Exceptions::Exception
    {
    public:

        ItemNotFound(
            size_t index,
            void* pointer = nullptr,
            char const* const file = 0,
            const int& line = -1) noexcept
            : Exception("Item with provided index/pointer didn't exist",
                file,
                line),
            m_Index(index),
            m_Ptr(pointer)
        {}

        ~ItemNotFound() noexcept = default;

    public:

        void const* GetPtr() { return m_Ptr; }

        const size_t& GetIndex() { return m_Index; }

    private:

        void* m_Ptr = nullptr;
        size_t m_Index = 0;

    };
}