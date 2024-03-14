#pragma once

namespace LUL_
{
	class LUL_EXPORT World
	{
	private:
		World() = default;

	public:
		static World& Get()
		{
			static World instance;
			return instance;
		}
		World(World&&) = delete;
		World(const World&) = delete;
		~World() = default;

#ifdef _LUL_DX12
	public:
		void InitializeWorld(std::shared_ptr<DX12::Renderer> renderer);

	public:
		std::shared_ptr<DX12::Renderer> GetRenderer() { return m_pRender; }

	private:
		std::shared_ptr<DX12::Renderer> m_pRender = std::shared_ptr<DX12::Renderer>(nullptr);

#endif // _LUL_DX12
	};
}