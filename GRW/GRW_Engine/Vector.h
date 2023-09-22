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

	Vector3 Negate() const
	{
		return *this * -1;
	}

	float* GetVal()
	{
		float val[3] = { x,y,z };
		return val;
	}


	static Vector3 zero()
	{
		return Vector3(0, 0, 0);
	}

	static Vector3 one()
	{
		return Vector3(1, 1, 1);
	}

	static Vector3 forward()
	{
		return Vector3(0, 0, 1);
	}

	static Vector3 right()
	{
		return Vector3(1, 0, 0);
	}

	static Vector3 up()
	{
		return Vector3(0, 1, 0);
	}

	Vector4 GetVec4(bool pos)
	{
		return Vector4(x, y, z, pos);
	}

};


struct Vector2
{
	Vector2() { x = 0, y = 0; }
	Vector2(float xIn, float yIn) : x(xIn), y(yIn) {}
	Vector2(Vector2 const& v)
	{
		x = v.x;
		y = v.y;
	}
	~Vector2() {}
	float x, y;

	//operator overloads
	Vector2 operator+(Vector2 const& v2) const
	{
		Vector2 result;
		result.x = x + v2.x;
		result.y = y + v2.y;
		return result;
	}
	Vector2 operator-(Vector2 const& v2) const
	{
		Vector2 result;
		result.x = x - v2.x;
		result.y = y - v2.y;
		return result;
	}
	Vector2 operator*(int const& f) const
	{
		Vector2 result;
		result.x = x * f;
		result.y = y * f;
		return result;
	}
	Vector2 operator*(Vector2 const& v2) const
	{
		Vector2 result;
		result.x = x * v2.x;
		result.y = y * v2.y;
		return result;
	}
	Vector2 operator/(float const& f) const
	{
		Vector2 result;
		result.x = x / f;
		result.y = y / f;
		return result;
	}
	void operator=(Vector2 const& v2)
	{
		this->x = v2.x;
		this->y = v2.y;
	}
	//common vector operations
	//dot
	float Dot(Vector2 const& v2) const
	{
		return (x * v2.x) + (y * v2.y);
	}
	float mag() const
	{
		return sqrt(x * x + y * y);
	}
	//sqrmagnitude 
	float sqrtmag() const
	{
		return x * x + y * y;
	}
	//normalize
	Vector2 GetNormalize() const
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
		std::string s = std::to_string(x) + ", " + std::to_string(y);
		return s;
	}
	//distance
	float Dist(Vector2 const& v2) const
	{
		Vector2 d = *this - v2;
		return d.mag();
	}
	//cosine angle in radians
	float Angle(Vector2 const& v2) const
	{
		float d = this->Dot(v2);
		float aM = this->mag();
		float bM = v2.mag();
		float angle = acos(d / (aM * bM));
		return angle;
	}

	Vector2 Negate() const
	{
		return *this * -1;
	}

	float* GetVal()
	{
		float val[2] = { x,y };
		return val;
	}
};

struct Vector4
{
	Vector4() { x = 0, y = 0, z = 0, w = 0; }
	Vector4(float xIn, float yIn, float zIn, float wIn) : x(xIn), y(yIn), z(zIn) , w(wIn) {}
	Vector4(Vector4 const& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}
	~Vector4() {}
	float x, y, z, w;

	//operator overloads
	Vector4 operator+(Vector4 const& v2) const
	{
		Vector4 result;
		result.x = x + v2.x;
		result.y = y + v2.y;
		result.z = z + v2.z;
		result.w = w + v2.w;
		return result;
	}
	Vector4 operator-(Vector4 const& v2) const
	{
		Vector4 result;
		result.x = x - v2.x;
		result.y = y - v2.y;
		result.z = z - v2.z;
		result.w = w - v2.w;
		return result;
	}
	Vector4 operator*(int const& f) const
	{
		Vector4 result;
		result.x = x * f;
		result.y = y * f;
		result.z = z * f;
		result.w = w * f;
		return result;
	}

	Vector4 operator*(Vector4 const& v2) const
	{
		Vector4 result;
		result.x = x * v2.x;
		result.y = y * v2.y;
		result.z = z * v2.z;
		result.w = w * v2.w;
		return result;
	}

	Vector4 operator/(float const& f) const
	{
		Vector4 result;
		result.x = x / f;
		result.y = y / f;
		result.z = z / f;
		result.w = w / f;
		return result;
	}
	void operator=(Vector4 const& v2)
	{
		this->x = v2.x;
		this->y = v2.y;
		this->z = v2.z;
		this->w = v2.w;
	}
	//common vector operations
	//dot
	float Dot(Vector4 const& v2) const
	{
		return (x * v2.x) + (y * v2.y) + (z * v2.z) + (w * v2.w);
	}

	//magnitude
	float mag() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}
	//sqrmagnitude 
	float sqrtmag() const
	{
		return x * x + y * y + z * z + w * w;
	}
	//normalize
	Vector4 GetNormalize() const
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
		std::string s = std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ", " + std::to_string(w);
		return s;
	}
	//distance
	float Dist(Vector4 const& v2) const
	{
		Vector4 d = *this - v2;
		return d.mag();
	}

	Vector4 Negate() const
	{
		return *this * -1;
	}


	float* GetVal()
	{
		float val[4] = { x,y,z,w };
		return val;
	}

	Vector3 GetVec3()
	{
		return Vector3(x, y, z);
	}
};