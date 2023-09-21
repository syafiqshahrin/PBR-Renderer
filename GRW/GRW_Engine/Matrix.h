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
	//get identity
	Matrix4x4 Identity()
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