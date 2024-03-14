#include "LUL_3D.hpp"

#include "Entity.hpp"

// Mesh ------------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Mesh::LoadMeshFromObj(char const* const path)
{
	LUL_::World::Get().GetRenderer()->CreateResourcesForMesh(this, MEM_KiB(64));
}

// Entity ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Entity::Update()
{
}

// DebugEntity -----------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::DX12::DebugEntity::DebugEntity()
{
	this->GetMesh()->LoadMeshFromObj("");

	// Define the geometry for a triangle.
	static float
		fRain1 = 0.0f,
		fRain2 = 0.5f,
		fRain3 = 0.25f;

	static float
		fRA1 = 0.0008f,
		fRA2 = 0.0002f,
		fRA3 = 0.0005f;

	if (fRain1 >= 1.0f ||
		fRain1 <= 0.0f)
		fRA1 = -fRA1;
	if (fRain2 >= 1.0f ||
		fRain2 <= 0.0f)
		fRA2 = -fRA2;
	if (fRain3 >= 1.0f ||
		fRain3 <= 0.0f)
		fRA3 = -fRA3;

	fRain1 += fRA1;
	fRain2 += fRA2;
	fRain3 += fRA3;

	Vertex triangleVertices[] =
	{
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },

		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain2, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain3, 1.0f } },

		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain2, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },

		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain2, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain2, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain2, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		// ---------------------------------------------------------------------------

		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },

		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain2, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain3, 1.0f } },

		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain2, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },

		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain2, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain2, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain2, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		// -----------------------------------------------------------------------------

		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },

		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain2, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain3, fRain2, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain3, 1.0f } },

		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain2, fRain2, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain3, fRain3, fRain3, 1.0f } },

		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 0.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain3, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain2, fRain2, fRain1, 1.0f } },
		{ { 0.0f, 1.0f, 0.0f, 1.0f }, { fRain2, fRain2, fRain2, 1.0f } },
		{ { 1.0f, 1.0f, 1.0f, 1.0f }, { fRain1, fRain2, fRain1, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f, 1.0f }, { fRain1, fRain2, fRain2, 1.0f } },

		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 1.0f, 1.0f }, { fRain2, fRain1, fRain3, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain3, fRain3, 1.0f } },
		{ { 1.0f, 0.0f, 1.0f, 1.0f }, { fRain3, fRain1, fRain1, 1.0f } },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, { fRain1, fRain1, fRain1, 1.0f } },
		{ { 1.0f, 0.0f, 0.0f, 1.0f }, { fRain2, fRain1, fRain2, 1.0f } }
	};
	const UINT vertexBufferSize = sizeof(triangleVertices);

	GetMesh()->GetRawBuffer()->Upload(
		triangleVertices,
		sizeof(Vertex),
		vertexBufferSize / sizeof(Vertex),
		4,
		Begin);

	World::Get().GetRenderer()->AddEntity(this);
}
