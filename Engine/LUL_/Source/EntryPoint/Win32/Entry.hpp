#pragma once

#ifdef _WIN32
#include "LUL_.h"

INT CALLBACK wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPWSTR cmdline, INT cmdshow){
    LUL_::AppProperties::Get();
    LUL_::Logger::Get();

    auto* app = LUL_::EntryApplication();

    app->Initialize();

    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        app->Update();
    }

    app->Destroy();

    LUL_PROFILER_TIMER_RESULTS();

    // In debug, make sure that logger finished write
#ifdef _DEBUG
    LUL_::Logger::Get().WaitForTraffic();
#endif // _DEBUG
}
#endif // _WIN32
