#include "Matrix.h"

Matrix3x3 Matrix4x4::GetMat3x3()
{
	float m3[3][3] =
	{
		{val[0][0],val[0][1],val[0][2]},
		{val[1][0],val[1][1],val[1][2]},
		{val[2][0],val[2][1],val[2][2]},
	};
	Matrix3x3 M(m3);
	return M;
}