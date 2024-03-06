#include "LUL_3D.hpp"

// -----------------------------------------------------------------------------
LUL_::L_Float4 LUL_::Math::Vector4Transform(
	const LUL_::L_Float4& v, 
	const LUL_::L_MatFloat4x4& m)
{
	L_Float4 o;

	o.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
	o.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
	o.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
	o.w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	if (o.w != 0.0f)
	{
		o.x /= o.w;
		o.y /= o.w;
		o.z /= o.w;
	}

	return o;
}
