#pragma once

namespace LUL_::DX12
{
	class LUL_EXPORT GpuBasedObject
	{
		LUL_::DX12::GpuBasedBuffer m_Buffer = LUL_::DX12::GpuBasedBuffer(nullptr);

	public:
		GpuBasedObject() = default;
		GpuBasedObject(const GpuBasedObject&) = default;
		GpuBasedObject(GpuBasedObject&&) = delete;
		~GpuBasedObject() = default;

	public:
		void SetGpuBuffer(GpuBasedBuffer b) { b = m_Buffer; }
	}; 

	class LUL_EXPORT Mesh 
		: public GpuBasedObject
	{
	private:
		DirectX::XMMATRIX m_Offset = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	public:
		Mesh() = default;
		Mesh(const Mesh&) = default;
		Mesh(Mesh&&) = delete;
		~Mesh() = default;

	public:
		void LoadMeshFromObj(char const* const path);
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
		std::shared_ptr<Mesh> GetMesh() { return m_pMesh; }

	};

	class LUL_EXPORT DebugEntity
		: public Entity
	{
	public:
		DebugEntity();
	};
}
