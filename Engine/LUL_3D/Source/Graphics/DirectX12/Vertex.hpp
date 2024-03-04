#pragma once

#ifdef _WIN64
namespace LUL_::Graphics::DX12
{
    static const D3D12_INPUT_ELEMENT_DESC VertexElementsDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    struct Vertex
    {
        float& operator[](size_t indx)
        {
            switch (indx)
            {
            case 0:
                return postion.x;

            case 1:
                return postion.y;

            case 2:
                return postion.z;

            case 3:
                return postion.w;

            default:
                throw LUL_::Exceptions::InvalidArg(LUL_EXCPT_HELPER());
            }
        }

        ::DirectX::XMFLOAT4 postion;
        ::DirectX::XMFLOAT4 color;
    };
}
#endif // _WIN64