#pragma once

namespace LUL_::DX12
{
	class LUL_EXPORT Mesh 
		: public GpuBasedObject
	{
	private:
		DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		std::vector<Vertex> m_CpuBuffer;

	public:
		Mesh() = default;
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = delete;
		~Mesh() = default;

	public:
		void LoadMeshFromObj(wchar_t const* const path);

		// Getters ---------------------------------------------------------------------
	public:
		std::vector<Vertex>& GetCpuBuffer() { return m_CpuBuffer; }
	};

	class LUL_EXPORT Entity
	{
	private:
		std::shared_ptr<Mesh> m_pMesh = std::make_shared<Mesh>();

	public:
		Entity() = default;
		Entity(const Entity&) = default;
		Entity(Entity&&) = default;
		~Entity() = default;

	public:
		void Update();

	public:
		std::shared_ptr<Mesh> GetMesh() { return m_pMesh; }

	};

	class LUL_EXPORT DebugEntity
		: public Entity
	{
	public:
		DebugEntity();
	};
}
