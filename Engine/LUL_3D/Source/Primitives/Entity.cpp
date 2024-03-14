#include "LUL_3D.hpp"

#include "Entity.hpp"

// Define the geometry for a triangle.
static float
fRain1 = 0.0f,
fRain2 = 0.5f,
fRain3 = 0.25f;

static float
fRA1 = 0.0008f,
fRA2 = 0.0002f,
fRA3 = 0.0005f;

LUL_::DX12::Vertex triangleVertices[] =
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

// Mesh ------------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Mesh::LoadMeshFromObj(char const* const path)
{
	m_CpuBuffer = triangleVertices;

	LUL_::World::Get().GetRenderer()->CreateResourcesForMesh(this, MEM_KiB(128));
}

// Entity ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Entity::Update()
{
	const UINT vertexBufferSize = sizeof(triangleVertices);

 	using namespace DirectX;
 
 	static const float fFov = 120.0f;
 	static const float fNear = 0.1f;
 	static const float fFar = 1000.0f;
 	const float fWidth = 1200.0f;
 	const float fHeight = 800.0f;
 	const float fAspectRatio = fWidth / fHeight;
 
 	XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(
 		XMConvertToRadians(fFov),
 		fAspectRatio,
 		fNear,
 		fFar);
 
 	// XMMATRIX projection = DirectX::XMMatrixOrthographicLH(
 	// 	vp.Width,
 	// 	vp.Height,
 	// 	fFar,
 	// 	fNear);
 
 	static float fTheta = 1.0f;
 	static float fThetaDirection = 0.005f;
 	fTheta += fThetaDirection;
 
 	XMVECTOR 
 		v,
 		result;
 
 	XMMATRIX 
 		rotationZ,
 		rotationX,
 		rotationY,
 		offset,
 		rotatedXZ,
 		rotatedXZY,
 		offsetedAndRotated,
 		finished;
 	
 	rotationZ = XMMatrixRotationZ(XMConvertToRadians(fTheta));
 	rotationX = XMMatrixRotationX(XMConvertToRadians(fTheta));
 	rotationY = XMMatrixRotationY(XMConvertToRadians(fTheta));
 	offset = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
 
 	// Rotate
 	rotatedXZ = XMMatrixMultiply(rotationX, rotationZ);
 	rotatedXZY = XMMatrixMultiply(rotatedXZ, rotationY);
 	offsetedAndRotated = XMMatrixMultiply(rotatedXZY, offset);
 	
 	finished = XMMatrixMultiply(offsetedAndRotated, projection);
 
	Vertex* resultVer = (Vertex*)malloc(vertexBufferSize);
 	float secondCubeOff = 0.0f;
	Vertex* raw = m_pMesh->GetCpuBuffer();
	memcpy_s(resultVer, vertexBufferSize, raw, vertexBufferSize);
 	for (int i = 0; i < m_pMesh->GetRawBuffer()->GetDataCount(); ++i)
 	{
 		if (i >= 36)
 			secondCubeOff = -3.5f;
 		if (i >= 72)
 			secondCubeOff = 3.5f;
 
 		v.m128_f32[0] = raw[i].Position.x + secondCubeOff;
 		v.m128_f32[1] = raw[i].Position.y + secondCubeOff;
 		v.m128_f32[2] = raw[i].Position.z + secondCubeOff;
 		v.m128_f32[3] = raw[i].Position.w;
 
 		// Calculate result
 		result = XMVector4Transform(v, finished);
 
  		if (result.m128_f32[2] < 0.0f)
  			result.m128_f32[2] = -result.m128_f32[2];
 
 		// Scale down
		resultVer[i].Position.x = result.m128_f32[0];
		resultVer[i].Position.y = result.m128_f32[1];
		resultVer[i].Position.z = result.m128_f32[2];
		resultVer[i].Position.w = /* 10.0f; */ result.m128_f32[3];
 		// L_LOG(L_INFO, L"%f %f %f %f", tri[0], tri[1], tri[2], tri[3]);
 	}

	GetMesh()->GetRawBuffer()->Upload(
		resultVer,
		sizeof(Vertex),
		vertexBufferSize / sizeof(Vertex),
		4,
		Begin);

	free(resultVer);
}

// DebugEntity -----------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::DX12::DebugEntity::DebugEntity()
{
	this->GetMesh()->LoadMeshFromObj("");

	const UINT vertexBufferSize = sizeof(triangleVertices);
	GetMesh()->GetRawBuffer()->Upload(
		triangleVertices,
		sizeof(Vertex),
		vertexBufferSize / sizeof(Vertex),
		4,
		Begin);

	World::Get().GetRenderer()->AddEntity(this);
}
