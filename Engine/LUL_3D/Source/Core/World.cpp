#include "LUL_3D.hpp"

#include "World.hpp"

// World -----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::World::InitializeWorld(std::shared_ptr<DX12::Renderer> renderer)
{
	m_pRender = renderer;
}
