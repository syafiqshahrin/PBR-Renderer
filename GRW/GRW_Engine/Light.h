#pragma once
#include "Vector.h"
struct Transform;

class Light
{
public:
	Light(Transform& tform);
	Light(Transform& tform, float i , Vector3 const &col);
	~Light();

	void SetPosition(Vector3 const &pos);
	void SetIntensity(float const &i);
	void SetColor(Vector3 const& col);
	Vector3 GetPosition();
	float GetIntensity();
	Vector3 GetColor();

	Vector3 color;
	float intensity;
private:
	Transform* transform;

};

class DirectLight : public Light
{
public:
	DirectLight(Transform& tform, float i, Vector3 const& col, Vector3 const& dir);
	void SetLightDirection(Vector3 const& dir);
	Vector3 GetLightDirection();
	Vector3 lightDirection;
private:
};

class PointLight : public Light
{
public:
	PointLight(Transform& tform, float i, Vector3 const& col, float rad);
	void SetLightRadius(float rad);
	float GetLightRadius();
	float radius;
private:
};