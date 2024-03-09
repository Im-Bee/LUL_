#pragma once

namespace LUL_::Graphics::DX12
{
	class LUL_EXPORT Mesh
	{
	private:

		DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	public:



	};

	class LUL_EXPORT Entity
	{
	private:

		std::shared_ptr<Mesh> m_Mesh = std::shared_ptr<Mesh>(nullptr);

	public:



	private:



	};
}
