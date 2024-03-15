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
void LUL_::DX12::Mesh::LoadMeshFromObj(wchar_t const* const path)
{
	LUL_PROFILER_TIMER_START();
	L_LOG(L_INFO, L"Loading mesh from file %lS", path);
	
	constexpr bool fromBottomToTop = true;
	char* fileBuf = nullptr;
	std::streamsize fileSize = 0;
	std::ifstream objFile(
		path, // Should be okay on windows
		std::ios_base::in | std::ios_base::ate);
	if (!objFile.is_open())
	{
		throw Exceptions::InvalidArg(LUL_EXCPT_HELPER());
	}
	fileSize = objFile.tellg();
	L_LOG(L_INFO, L"File size %lld", (long long)fileSize);
	
	fileBuf = (char*)malloc(sizeof(char) * fileSize);
	if (!fileBuf)
	{
		throw Exceptions::Internal(LUL_EXCPT_HELPER());
	}
	
	objFile.seekg(std::ios_base::beg);
	for (;!objFile.eof();)
	{
		objFile.read(fileBuf + objFile.tellg(), 1024);
	}
	
	int_fast32_t 
		lineSize = 0,
		tagOffset = 0,
		k,
		indexOfFloatRead = 0;
	float vertexRead[8] = {
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f };
	std::string stringified = std::string();
	for (int64_t i = (fileSize - 1); i >= 0; --i)
	{
		// Search for next line
		if (fileBuf[i] != '\n')
		{
			lineSize++;
			continue;
		}
			
		// Check the tag at the beggining of the file
		// Start from index 1 to skip '\n'
		for (tagOffset = 1; tagOffset < lineSize; ++tagOffset)
		{
			char const* const ch = &fileBuf[i + tagOffset];
			if (*ch == ' ')
				break;
	
			stringified += *ch;
		}
	
		// If it's not line with vertices skip
		if (stringified != "v")
		{
			// Reset stringified
			stringified = "";
			lineSize = 0;
			continue;
		}
		
		// Reset stringified once before reading
		stringified = "";
		const char* ch = nullptr;
		for (k = tagOffset + 1; k <= lineSize; ++k)
		{
			if (k < lineSize)
			{
				ch = &fileBuf[i + k];
			}
	
			if ((*ch < '0' || *ch > '9') && *ch != '-' && *ch != '.' && *ch != ' ')
			{
				L_LOG(L_WARNING, L"Found incorect value in obj file | offset: %d", (i + k));
				stringified += '0';
				continue;
			}
			
			if (k < lineSize &&
				*ch != ' ')
			{
				stringified += *ch;
				continue;
			}
			
			vertexRead[indexOfFloatRead++] = std::stof(stringified);
			stringified = "";
		}
	
		if (indexOfFloatRead <= 3)
		{
			m_CpuBuffer.push_back(Vertex({
				vertexRead[0], vertexRead[1], vertexRead[2], 1.0f,
				0.5f, 0.5f, 0.5f, 0.5f }));
		}
		else if (indexOfFloatRead == 4)
		{
			m_CpuBuffer.push_back(Vertex({
				vertexRead[0], vertexRead[1], vertexRead[2], vertexRead[3],
				0.5f, 0.5f, 0.5f, 0.5f }));
		}
		else if (indexOfFloatRead > 4)
		{
			m_CpuBuffer.push_back(Vertex({
				vertexRead[0], vertexRead[1], vertexRead[2], vertexRead[3],
				vertexRead[4], vertexRead[5], vertexRead[6], vertexRead[7] }));
		}
	
		indexOfFloatRead = 0;
		// Reset stringified
		stringified = "";
		lineSize = 0;
	}
	

	// // Use debug buffer instead
	// m_CpuBuffer.clear();
	// for (int i = 0; i < sizeof(triangleVertices) / sizeof(Vertex); ++i)
	// {
	// 	m_CpuBuffer.push_back(triangleVertices[i]);
	// }

	// // Reverse the mesh
	// std::vector<Vertex> copy;
	// for (int i = 0; i < m_CpuBuffer.size(); ++i)
	// {
	// 	copy.push_back(m_CpuBuffer[m_CpuBuffer.size() - i - 1]);
	// }
	// m_CpuBuffer = copy;

	LUL_::World::Get().GetRenderer()->CreateResourcesForMesh(
		this, 
		m_CpuBuffer.size() * sizeof(Vertex));
}

// Entity ----------------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
void LUL_::DX12::Entity::Update()
{
	if (m_pMesh->GetCpuBuffer().empty())
		return;

	const UINT vertexBufferSize = sizeof(triangleVertices);

 	using namespace DirectX;
 
 	static const float fFov = 120.0f;
 	static const float fNear = 0.1f;
 	static const float fFar = 1000.0f;
 	static const float fWidth = World::Get().GetRenderer()->GetViewPort().Width;
	static const float fHeight = World::Get().GetRenderer()->GetViewPort().Height;
 	static const float fAspectRatio = fWidth / fHeight;
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
 
 	static float fTheta = -1.0f;
 	static float fThetaDirection = 0.05f;
 	fTheta -= fThetaDirection;
 
	static float fqwe = 2.5f;
	static float fqwer = 0.0005f;
	fqwe += fqwer;

	if (fqwe > 5.0f || fqwe < -5.0f)
		fqwer = -fqwer;

 	XMVECTOR 
 		v,
 		result;
 
 	XMMATRIX 
 		rotationZ,
 		rotationX,
 		rotationY,
 		offset,
		scaled,
 		rotatedXZ,
 		rotatedXZY,
		offsetedAndRotated,
		offsetedAndRotatedAndScaled,
 		finished;
 	

	rotationX = XMMatrixRotationX(XMConvertToRadians(0.0f));
	rotationZ = XMMatrixRotationZ(XMConvertToRadians(0.0f));
	rotationY = XMMatrixRotationY(XMConvertToRadians(fTheta));
	offset = XMMatrixTranslation(fqwe, -2.5f, 5.0f);
	scaled = XMMatrixScaling(0.115f, 0.115f, 0.115f);

	// Rotate
	rotatedXZ = XMMatrixMultiply(scaled, rotationX);
	rotatedXZY = XMMatrixMultiply(rotatedXZ, rotationZ);
	offsetedAndRotated = XMMatrixMultiply(rotatedXZY, rotationY);
	offsetedAndRotatedAndScaled = XMMatrixMultiply(offsetedAndRotated, offset);

 	finished = XMMatrixMultiply(offsetedAndRotatedAndScaled, projection);
	
	const uint64_t resultVerSize = sizeof(Vertex) * m_pMesh->GetVertexInstanceCount();
	Vertex* resultVer = (Vertex*) malloc(resultVerSize);
	int64_t r = 0;
 	for (auto& i : m_pMesh->GetCpuBuffer())
 	{
 		v.m128_f32[0] = i.Position.x;
 		v.m128_f32[1] = i.Position.y;
 		v.m128_f32[2] = i.Position.z;
 		v.m128_f32[3] = i.Position.w;
 
 		// Calculate result
 		result = XMVector4Transform(v, finished);
 
		resultVer[r].Position.x = result.m128_f32[0];
		resultVer[r].Position.y = result.m128_f32[1];
		resultVer[r].Position.z = result.m128_f32[2];
		resultVer[r].Position.w = result.m128_f32[3];
 		// L_LOG(L_INFO, L"%f %f %f %f", tri[0], tri[1], tri[2], tri[3]);
		++r;
 	}

	GetMesh()->GetRawBuffer()->Upload(
		resultVer,
		sizeof(Vertex),
		m_pMesh->GetVertexInstanceCount(),
		std::alignment_of<Vertex>().value,
		Begin);

	free(resultVer);
}

// DebugEntity -----------------------------------------------------------------
// Public ----------------------------------------------------------------------
// -----------------------------------------------------------------------------
LUL_::DX12::DebugEntity::DebugEntity()
	: Entity()
{
	this->GetMesh()->LoadMeshFromObj(LUL_::AppProperties::Get().CreatePathInKnownDir(
		KnownDirs::CurrentProject, 
		L"Assets\\DirectX12\\Obj\\DebugDuck.obj").c_str());
}
