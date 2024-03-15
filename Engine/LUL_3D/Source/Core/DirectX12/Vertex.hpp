#pragma once

#ifdef _WIN64
namespace LUL_::DX12
{
    static const D3D12_INPUT_ELEMENT_DESC VertexElementsDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    struct Vertex
    {
        Vertex() = default;
        Vertex(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8)
        {
            Position = { f1, f2, f3, f4 };
            Color = { f5, f6, f7, f8 };
        }
        Vertex(::DirectX::XMFLOAT4 p, ::DirectX::XMFLOAT4 c)
            : Position(p), Color(c)
        {
        }
        ~Vertex() = default;

        ::DirectX::XMFLOAT4 Position;
        ::DirectX::XMFLOAT4 Color;

        float& operator[](const size_t& indx)
        {
            switch (indx)
            {
            case 0:
                return Position.x;

            case 1:
                return Position.y;

            case 2:
                return Position.z;

            case 3:
                return Position.w;

            default:
                throw LUL_::Exceptions::InvalidArg(LUL_EXCPT_HELPER());
            }
        }
    };
}
#endif // _WIN64