#pragma once
#include "Vector.h"


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
				M.val[y][x] *= val[y][0] * m2.val[0][x] + val[y][1] * m2.val[1][x] + val[y][2] * m2.val[2][x] + val[y][3] * m2.val[3][x];
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

	//get inverse
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
			{cosZ, sinZcosX,  sinZsinX, 0}
			{sinZ, cosZcosX, -cosZsinX, 0} 
			{	0,	   sinX,	  cosX, 0} ----
			{	0,		   0,        0, 0}
		}

		(ZX)Y =
		{
			{cosZcosY + -sinZsinXsinY, sinZcosX, cosZsinY + sinZsinXcosY, 0}
			{sinZcosY + cosZsinXsinY, cosZcosX, sinZsinY - cosZsinXcosY, 0}
			{			   -cosXsinY,      sinX,              cosXcosY, 0}
			{                      0,         0,                    0, 0}
		}

		*/


		float rMatrix[4][4] =
		{
			{cos(a.z) * cos(a.y) - sin(a.z) * sin(a.x) * sin(a.y) , sin(a.z) * cos(a.x), cos(a.z) * sin(a.y) + sin(a.z) * sin(a.x) * cos(a.y), 0},
			{sin(a.x) * cos(a.y) + cos(a.z) * sin(a.x) * sin(a.y), cos(a.z) * cos(a.x), sin(a.z) * sin(a.y) - cos(a.z) * sin(a.x) * cos(a.y), 0},
			{-1 * cos(a.x) * sin(a.y), sin(a.x), cos(a.x) * cos(a.y), 0},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix);
		return M;
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
			{0, 0, 0, pos.x},
			{0, 0, 0, pos.y},
			{0, 0, 0, pos.z},
			{0, 0, 0, 1}
		};
		Matrix4x4 M(rMatrix);
		return M;
	}

	//Get Projection Matrix
	static Matrix4x4 GetProjectionMatrix()
	{
		float rMatrix[4][4] =
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
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



	//get projection matrix
};