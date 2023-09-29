#pragma once
#include "Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Debug.h"

struct Matrix3x3;

struct Matrix4x4
{

	//Using column major/column vectors for matrix multiplication
	Matrix4x4() {  }
	Matrix4x4(float matrix[4][4]) 
	{  
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				val[y][x] = matrix[y][x];
			}
		}
	}
	Matrix4x4(Matrix4x4 const &matrix)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				val[y][x] = matrix.val[y][x];
			}
		}
	}
	~Matrix4x4() {}
	float val[4][4] = { 0 }; //[y][x] row major

	//operator overloads
	//add
	//subtract
	//multiply scalar
	Matrix4x4 operator*(float const& f) const
	{
		Matrix4x4 M;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				M.val[y][x] *= f;
			}
		}
		return M;
	}
	//multiply matrix
	Matrix4x4 operator*(Matrix4x4 const& m2) const
	{

		//formula M[y][x] = sum of m1[y][i] * m2[i][x] ( where i starts from 0 and goes to max Column value, in this case 4)
		Matrix4x4 M;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				M.val[y][x] = val[y][0] * m2.val[0][x] + val[y][1] * m2.val[1][x] + val[y][2] * m2.val[2][x] + val[y][3] * m2.val[3][x];
			}
		}
		return M;
	}
	//multiply vec4
	Vector4 operator*(Vector4 const& vIn) const
	{
		Vector4 V;

		V.x = val[0][0] * vIn.x + val[0][1] * vIn.y + val[0][2] * vIn.z + val[0][3] * vIn.w; ;
		V.y = val[1][0] * vIn.x + val[1][1] * vIn.y + val[1][2] * vIn.z + val[1][3] * vIn.w; ;
		V.z = val[2][0] * vIn.x + val[2][1] * vIn.y + val[2][2] * vIn.z + val[2][3] * vIn.w; ;
		V.w = val[3][0] * vIn.x + val[3][1] * vIn.y + val[3][2] * vIn.z + val[3][3] * vIn.w; ;

		return V;
	}

	//get index
	float operator() (int y, int x) const
	{
		return val[y][x];
	}

	//operations

	//get determinant
	float GetDeterminant()
	{
		Vector3 a = Vector3(val[0][0], val[1][0],val[2][0]);
		Vector3 b = Vector3(val[0][1], val[1][1],val[2][1]);
		Vector3 c = Vector3(val[0][2], val[1][2],val[2][2]);
		Vector3 d = Vector3(val[0][3], val[1][3],val[2][3]);

		float x = val[3][0];
		float y = val[3][1];
		float z = val[3][2];
		float w = val[3][3];

		Vector3 s = a.Cross(b);
		Vector3 t = c.Cross(d);
		Vector3 u = (a * y) - ( b * x);
		Vector3 v = (c * w) - ( d * z);

		float det = s.Dot(v) + t.Dot(u);
		
		/*
		//calculate cofactors of the first row
		float c11 = (val[1][1] * (val[2][2] * val[3][3] - val[2][3] * val[3][2])) + (val[1][2] * (val[2][3] * val[3][1] - val[2][1] * val[3][3]))
			+ (val[1][3] * (val[2][1] * val[3][2] - val[2][2] * val[3][1]));
		float c12 = (val[1][0] * (val[2][2] * val[3][3] - val[2][3] * val[3][2])) + (val[1][2] * (val[2][3] * val[3][0] - val[2][0] * val[3][3]))
			+ (val[1][3] * (val[2][0] * val[3][2] - val[2][2] * val[3][0]));
		float c13 = (val[1][0] * (val[2][1] * val[3][3] - val[2][3] * val[3][1])) + (val[1][1] * (val[2][1] * val[3][3] - val[2][0] * val[3][3]))
			+ (val[1][3] * (val[2][0] * val[3][1] - val[2][1] * val[3][0]));
		float c14 = (val[1][0] * (val[2][1] * val[3][2] - val[2][2] * val[3][1])) + (val[1][1] * (val[2][2] * val[3][0] - val[2][0] * val[3][2]))
			+ (val[1][2] * (val[2][0] * val[3][1] - val[2][1] * val[3][0]));

		float det2 = c11 - c12 + c13 - c14;
		DEBUG("Det2 = " << det2);
		*/

		DEBUG("Det = " << det);

		return det;
	}
	//get inverse
	Matrix4x4 GetInverse()
	{
		Vector3 a = Vector3(val[0][0], val[1][0], val[2][0]);
		Vector3 b = Vector3(val[0][1], val[1][1], val[2][1]);
		Vector3 c = Vector3(val[0][2], val[1][2], val[2][2]);
		Vector3 d = Vector3(val[0][3], val[1][3], val[2][3]);

		float x = val[3][0];
		float y = val[3][1];
		float z = val[3][2];
		float w = val[3][3];

		Vector3 s = a.Cross(b);
		Vector3 t = c.Cross(d);
		Vector3 u = (a * y) - (b * x);
		Vector3 v = (c * w) - (d * z);

		float det = s.Dot(v) + t.Dot(u);

		float invDet = 1.0f / det;

		s *= invDet;
		t *= invDet;
		u *= invDet;
		v *= invDet;

		Vector3 r0 = b.Cross(v) + t * y;
		Vector3 r1 = v.Cross(a) - t * x;
		Vector3 r2 = d.Cross(u) + s * w;
		Vector3 r3 = u.Cross(c) - s * z;


		float iMatrix[4][4] =
		{
			{r0.x, r0.y, r0.z, -b.Dot(t)},
			{r1.x, r1.y, r1.z, a.Dot(t)},
			{r2.x, r2.y, r2.z, -d.Dot(s)},
			{r3.x, r3.y, r3.z, c.Dot(s)}
		};
		Matrix4x4 M(iMatrix);
		return M;
	}
	//get transpose
	Matrix4x4 Transpose() const
	{
		/*
		float v[4][4] = {
			{val[0][0], val[1][0], val[2][0], val[3][0]},
			{val[0][1], val[1][1], val[2][1], val[3][1]},
			{val[0][2], val[1][2], val[2][2], val[3][2]},
			{val[0][3], val[1][3], val[2][3], val[3][3]}
		};
		*/
		Matrix4x4 Mt;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				Mt.val[y][x] = val[x][y];
			}
		}
		
		return Mt;
	}
	
	//static funcs
	//get rotation matrix
	static Matrix4x4 GetRotationMatrix(Vector3 a)
	{

		/*
		rotation around x axis:

		{
			{1,    0,     0, 0},
			{0, cosX, -sinX, 0},
			{0, sinX, cosX,  0},
			{0,    0,    0,  0}
		}
		
		rotation around y axis

		{
			{ cosY, 0, sinY, 0},
			{    0, 1,    0, 0},
			{-sinY, 0, cosY, 0},
			{    0, 0,    0, 0}
		}
		
		rotation around Z axis

		{
			{cosZ, -sinZ, 0, 0}
			{sinZ,  cosZ, 0, 0} 
			{   0,     0, 1, 0}
			{   0,     0, 0, 0}
		}
		
		Composed rotation matrix using ZXY order

		ZX =
		{
			{cosZ, -sinZcosX,  sinZsinX, 0}
			{sinZ, cosZcosX, -cosZsinX, 0} 
			{	0,	   sinX,	  cosX, 0} ----
			{	0,		   0,        0, 0}
		}

		(ZX)Y =
		{
			{cosZcosY - sinZsinXsinY, -sinZcosX, cosZsinY + sinZsinXcosY, 0}
			{sinZcosY + cosZsinXsinY, cosZcosX, sinZsinY - cosZsinXcosY, 0}
			{			   -cosXsinY,      sinX,              cosXcosY, 0}
			{                      0,         0,                    0, 0}
		}

		*/

		float cx = cos(a.x * M_PI / 180);
		float cy = cos(a.y * M_PI / 180);
		float cz = cos(a.z * M_PI / 180);

		float sx = sin(a.x * M_PI / 180);
		float sy = sin(a.y * M_PI / 180);
		float sz = sin(a.z * M_PI / 180);

		float x[4][4] =
		{
			{1,    0,     0, 0},
			{0, cx, -sx, 0},
			{0, sx, cx,  0},
			{0,    0,    0,  1}
		};
		float y[4][4] =
		{
			{cy, 0, sy, 0},
			{0, 1, 0, 0},
			{-sy, 0, cy, 0},
			{0, 0, 0, 1}
		};
		float z[4][4] =
		{
			{cz, -sz, 0, 0},
			{sz,  cz, 0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		};

		Matrix4x4 xM(x);
		Matrix4x4 yM(y);
		Matrix4x4 zM(z);

		Matrix4x4 rotM = yM * (zM * xM);
		/*
		float rMatrix[4][4] =
		{
			{cz * cy - sz * sx * sy , -sz * cx, cz * sy + sz * sx * cy, 0},
			{sx * cy + cz * sx * sy, cz * cx, sz * sy - cz * sx * cy, 0},
			{-cx * sy, sx, cx * cy, 0},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix);
		*/
		return rotM;
	}

	//get scale matrix
	static Matrix4x4 GetScalingMatrix(Vector3 scale)
	{
		float rMatrix[4][4] =
		{
			{scale.x, 0, 0, 0},
			{0, scale.y, 0, 0},
			{0, 0, scale.z, 0},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix);
		return M;
	}

	//get translation matrix
	static Matrix4x4 GetTranslationMatrix(Vector3 pos)
	{
		float rMatrix[4][4] =
		{
			{1, 0, 0, pos.x},
			{0, 1, 0, pos.y},
			{0, 0, 1, pos.z},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix);
		return M;
	}

	//Get Ortho Projection Matrix
	static Matrix4x4 GetOrthoProjectionMatrix(Vector3 minBounds, Vector3 maxBounds, Vector2 sDimensions)
	{
		float aspect =  sDimensions.y/ sDimensions.x;
		float wInv = 1.0f / (maxBounds.x - minBounds.x);
		float hInv = 1.0f / (maxBounds.y - minBounds.y);
		float dInv = 1.0f / (maxBounds.z - minBounds.z);

		float rMatrix[4][4] =
		{
			{wInv * 2.0f * aspect, 0, 0, -(maxBounds.x + minBounds.x) * wInv},
			{0, hInv * 2.0f, 0, -(maxBounds.y + minBounds.y) * hInv},
			{0, 0, dInv, -minBounds.z * dInv},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix); 
		return M;
	}

	//Get Pers Projection Matrix
	static Matrix4x4 GetPerspectiveProjectionMatrix(float fov, float nearP, float farP, float aspectRatio)
	{
		float fovRad = fov * M_PI / 180;
		//float test = tan(fovRad * 0.5f);
		//DEBUG(aspectRatio);
		float g = 1.0f / tan(fovRad * 0.5f);
		float k = farP / (farP - nearP);
		float rMatrix[4][4] =
		{
			{g * aspectRatio, 0, 0, 0},
			{0, g, 0, 0},
			{0, 0, k, -nearP * k},
			{0, 0, 1, 0}
		};
		Matrix4x4 M(rMatrix);
		return M;
	}
	//get identity
	static Matrix4x4 Identity()
	{
		float val[4][4] =
		{
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
		};
		Matrix4x4 M(val);// = Matrix4x4(val);
		
		return M;
	}

	void GetMatrixFloatArray(float *f)
	{
		int i = 0;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				f[i] = val[y][x];
				i++;
			}
		}
	}

	Matrix3x3 GetMat3x3();
};

struct Matrix3x3
{

	//Using column major/column vectors for matrix multiplication
	Matrix3x3() {  }
	Matrix3x3(float matrix[3][3])
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				val[y][x] = matrix[y][x];
			}
		}
	}
	Matrix3x3(Matrix3x3 const& matrix)
	{
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				val[y][x] = matrix.val[y][x];
			}
		}
	}
	~Matrix3x3() {}
	float val[3][3] = { 0 }; //[y][x] row major

	//operator overloads
	//add
	//subtract
	//multiply scalar
	Matrix3x3 operator*(float const& f) const
	{
		Matrix3x3 M;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				M.val[y][x] *= f;
			}
		}
		return M;
	}
	//multiply matrix
	Matrix3x3 operator*(Matrix3x3 const& m2) const
	{

		//formula M[y][x] = sum of m1[y][i] * m2[i][x] ( where i starts from 0 and goes to max Column value, in this case 4)
		Matrix3x3 M;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				M.val[y][x] = val[y][0] * m2.val[0][x] + val[y][1] * m2.val[1][x] + val[y][2] * m2.val[2][x];
			}
		}
		return M;
	}
	//multiply vec4
	Vector3 operator*(Vector3 const& vIn) const
	{
		Vector3 V;

		V.x = val[0][0] * vIn.x + val[0][1] * vIn.y + val[0][2] * vIn.z;
		V.y = val[1][0] * vIn.x + val[1][1] * vIn.y + val[1][2] * vIn.z;
		V.z = val[2][0] * vIn.x + val[2][1] * vIn.y + val[2][2] * vIn.z;
		//V.w = val[3][0] * vIn.x + val[3][1] * vIn.y + val[3][2] * vIn.z + val[3][3] * vIn.w; ;

		return V;
	}

	//get index
	float operator() (int y, int x) const
	{
		return val[y][x];
	}

	//operations

	//get determinant
	float GetDeterminant()
	{
		return 0;
	}
	//get inverse
	
	Matrix3x3 GetInverse()
	{
		Vector3 a = Vector3(val[0][0], val[1][0], val[2][0]);
		Vector3 b = Vector3(val[0][1], val[1][1], val[2][1]);
		Vector3 c = Vector3(val[0][2], val[1][2], val[2][2]);


		Vector3 r0 = b.Cross(c);
		Vector3 r1 = c.Cross(a);
		Vector3 r2 = a.Cross(b);


		float invDet = 1.0f / r2.Dot(c);


		float iMatrix[3][3] =
		{
			{r0.x * invDet, r0.y * invDet, r0.z * invDet},
			{r1.x * invDet, r1.y * invDet, r1.z * invDet},
			{r2.x * invDet, r2.y * invDet, r2.z * invDet}
		};
		Matrix3x3 M(iMatrix);
		return M;
	}
	
	//get transpose
	Matrix3x3 Transpose() const
	{
		/*
		float v[4][4] = {
			{val[0][0], val[1][0], val[2][0], val[3][0]},
			{val[0][1], val[1][1], val[2][1], val[3][1]},
			{val[0][2], val[1][2], val[2][2], val[3][2]},
			{val[0][3], val[1][3], val[2][3], val[3][3]}
		};
		*/
		Matrix3x3 Mt;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				Mt.val[y][x] = val[x][y];
			}
		}

		return Mt;
	}

	//static funcs
	//get rotation matrix
	static Matrix3x3 GetRotationMatrix(Vector3 a)
	{

		/*
		rotation around x axis:

		{
			{1,    0,     0, 0},
			{0, cosX, -sinX, 0},
			{0, sinX, cosX,  0},
			{0,    0,    0,  0}
		}

		rotation around y axis

		{
			{ cosY, 0, sinY, 0},
			{    0, 1,    0, 0},
			{-sinY, 0, cosY, 0},
			{    0, 0,    0, 0}
		}

		rotation around Z axis

		{
			{cosZ, -sinZ, 0, 0}
			{sinZ,  cosZ, 0, 0}
			{   0,     0, 1, 0}
			{   0,     0, 0, 0}
		}

		Composed rotation matrix using ZXY order

		ZX =
		{
			{cosZ, -sinZcosX,  sinZsinX, 0}
			{sinZ, cosZcosX, -cosZsinX, 0}
			{	0,	   sinX,	  cosX, 0} ----
			{	0,		   0,        0, 0}
		}

		(ZX)Y =
		{
			{cosZcosY - sinZsinXsinY, -sinZcosX, cosZsinY + sinZsinXcosY, 0}
			{sinZcosY + cosZsinXsinY, cosZcosX, sinZsinY - cosZsinXcosY, 0}
			{			   -cosXsinY,      sinX,              cosXcosY, 0}
			{                      0,         0,                    0, 0}
		}

		*/

		float cx = cos(a.x * M_PI / 180);
		float cy = cos(a.y * M_PI / 180);
		float cz = cos(a.z * M_PI / 180);

		float sx = sin(a.x * M_PI / 180);
		float sy = sin(a.y * M_PI / 180);
		float sz = sin(a.z * M_PI / 180);

		float x[3][3] =
		{
			{1,    0,     0},
			{0, cx, -sx},
			{0, sx, cx}
		};
		float y[3][3] =
		{
			{cy, 0, sy},
			{0, 1, 0},
			{-sy, 0, cy}
		};
		float z[3][3] =
		{
			{cz, -sz, 0},
			{sz,  cz, 0},
			{0, 0, 1}

		};

		Matrix3x3 xM(x);
		Matrix3x3 yM(y);
		Matrix3x3 zM(z);

		Matrix3x3 rotM = yM * (zM * xM);
		/*
		float rMatrix[4][4] =
		{
			{cz * cy - sz * sx * sy , -sz * cx, cz * sy + sz * sx * cy, 0},
			{sx * cy + cz * sx * sy, cz * cx, sz * sy - cz * sx * cy, 0},
			{-cx * sy, sx, cx * cy, 0},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix);
		*/
		return rotM;
	}

	//get scale matrix
	static Matrix3x3 GetScalingMatrix(Vector3 scale)
	{
		float rMatrix[3][3] =
		{
			{scale.x, 0, 0},
			{0, scale.y, 0},
			{0, 0, scale.z},
		};
		Matrix3x3 M(rMatrix);
		return M;
	}

	//get identity
	static Matrix3x3 Identity()
	{
		float val[3][3] =
		{
			{1,0,0},
			{0,1,0},
			{0,0,1}
		};
		Matrix3x3 M(val);// = Matrix4x4(val);

		return M;
	}

	void GetMatrixFloatArray(float* f)
	{
		int i = 0;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				f[i] = val[y][x];
				i++;
			}
		}
	}

	Matrix4x4 GetMat4x4()
	{
		float m4[4][4] =
		{
			{val[0][0],val[0][1],val[0][2], 0},
			{val[1][0],val[1][1],val[1][2], 0},
			{val[2][0],val[2][1],val[2][2], 0},
			{0, 0, 0, 0}
		};
		Matrix4x4 M(m4);
		return M;
	}
};