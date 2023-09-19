#pragma once
#include <cmath>
#include <string>

struct Vector3
{
	Vector3() { x = 0, y = 0, z = 0; }
	Vector3(float xIn, float yIn, float zIn) : x(xIn), y(yIn), z(zIn) {}
	Vector3(Vector3 const &v)
	{
		x = v.x; 
		y = v.y; 
		z = v.z;
	}
	~Vector3() {}
	float x, y, z;

	//operator overloads
	Vector3 operator+(Vector3 const& v2) const
	{
		Vector3 result;
		result.x = x + v2.x;
		result.y = y + v2.y;
		result.z = z + v2.z;
		return result;
	}
	Vector3 operator-(Vector3 const& v2) const
	{
		Vector3 result;
		result.x = x - v2.x;
		result.y = y - v2.y;
		result.z = z - v2.z;
		return result;
	}
	Vector3 operator*(int const& f) const
	{
		Vector3 result;
		result.x = x * f;
		result.y = y * f;
		result.z = z * f;
		return result;
	}
	Vector3 operator*(float const& f) const
	{
		Vector3 result;
		result.x = x * f;
		result.y = y * f;
		result.z = z * f;
		return result;
	}
	Vector3 operator*(Vector3 const& v2) const
	{
		Vector3 result;
		result.x = x * v2.x;
		result.y = y * v2.y;
		result.z = z * v2.z;
		return result;
	}
	Vector3 operator/(float const& f) const 
	{
		Vector3 result;
		result.x = x / f;
		result.y = y / f;
		result.z = z / f;
		return result;
	}
	void operator=(Vector3 const& v2)
	{
		this->x = v2.x;
		this->y = v2.y;
		this->z = v2.z;
	}
	//common vector operations
	//dot
	float Dot(Vector3 const& v2) const
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z);
	}
	//cross
	Vector3 Cross(Vector3 const& v2) const
	{
		Vector3 result;
		result.x = y * v2.z - z * v2.y;
		result.y = z * v2.x - x * v2.z;
		result.z = x * v2.y - y * v2.x;
		return result;
	}
	//magnitude
	float mag() const
	{
		return sqrt(x * x + y * y + z * z);
	}
	//sqrmagnitude 
	float sqrtmag() const
	{
		return x * x + y * y + z * z;
	}
	//normalize
	Vector3 GetNormalize() const
	{
		float m = mag();
		return *this / m;
	}
	void Normalize()
	{
		float m = mag();
		*this = *this / m;
	}

	std::string prnt() const
	{
		std::string s = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
		return s;
	}
	//distance
	float Dist(Vector3 const& v2) const
	{
		Vector3 d = *this - v2;
		return d.mag();
	}
	//cosine angle in radians
	float Angle(Vector3 const &v2) const
	{
		float d = this->Dot(v2);
		float aM = this->mag();
		float bM = v2.mag();
		float angle = acos(d / (aM * bM));
		return angle;
	}
};


struct Vector2
{

};

struct Vector4
{

};