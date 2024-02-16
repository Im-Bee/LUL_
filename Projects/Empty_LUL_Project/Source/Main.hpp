#include "PCH.hpp"

#include "Primitives/EmptyWindow.hpp"

int main()
{
    LUL_::AppProperties::Get();
    LUL_::AppProperties::Get().CreatePath(L"C:\\Home\\Desktop\\Results\\");
    LUL_::Logger::Get();

    LUL_::EmptyWindow i;

    LUL_PROFILER_TIMER_SET_OUTPUT_DIR(L"C:\\Home\\Desktop\\Results\\");
    LUL_PROFILER_TIMER_RESULTS();
}