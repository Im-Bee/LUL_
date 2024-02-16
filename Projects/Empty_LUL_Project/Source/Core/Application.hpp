#pragma once

#include "EntryPoint/Win32/Entry.hpp"

class Application
    : public LUL_::IApplication
{
public:

    Application() = default;

    ~Application() = default;

public:

    void Initialize() override;

    void Update() override;

    void Destroy() override;

};
