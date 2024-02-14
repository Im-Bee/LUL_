#pragma once



namespace LUL_::Exceptions
{
    class LUL_EXPORT Exception 
        : private std::exception
    {
    public:

        Exception() noexcept = default;
        
        explicit Exception(char const* const msg,
            char const* const file = 0,
            const int& line = -1) noexcept
            : exception(msg)
        {}

        ~Exception() noexcept = default;

    };

    class LUL_EXPORT NotImplemented
        : public LUL_::Exceptions::Exception
    {
    public:

        NotImplemented(char const* const file = 0,
            const int& line = -1) noexcept
            : Exception("This function isn't implemented on current system", 
                file, 
                line)
        {}

        ~NotImplemented() noexcept = default;
    };

    class LUL_EXPORT Internal
        : public LUL_::Exceptions::Exception
    {
    public:

        Internal(char const* const file = 0,
            const int& line = -1) noexcept
            : Exception("LUL_ coulnd't handle the heat and failed somewhere along the way", 
                file, 
                line)
        {}

        ~Internal() noexcept = default;
    };
}