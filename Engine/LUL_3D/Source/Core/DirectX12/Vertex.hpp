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