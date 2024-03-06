#pragma once

namespace LUL_
{
    template<typename T>
    struct LUL_EXPORT L_Vec2
    {
        T x;
        T y;
    };

    template<typename T>
    struct LUL_EXPORT L_Vec3
    {
        T x;
        T y;
        T z;
    };

    template<typename T>
    struct LUL_EXPORT L_Vec4
    {
        T x;
        T y;
        T z;
        T w;
    };

    struct LUL_EXPORT L_Float4
    {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
        float w = 0.0f;
    };

    template<typename T>
    struct LUL_EXPORT L_Mat4x4
    {
        T m[4][4] = { 0 };
    };

    struct LUL_EXPORT L_MatFloat4x4
    {
        float m[4][4] = { 0 };
    };
}