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

private:
	Transform* transform;
	float intensity;
	Vector3 color;

};

class DirectLight : public Light
{
public:
	DirectLight(Transform& tform, float i, Vector3 const& col, Vector3 const& dir);
	void SetLightDirection(Vector3 const& dir);
	Vector3 GetLightDirection();
private:
	Vector3 lightDirection;
};

class PointLight : public Light
{
public:
	PointLight(Transform& tform, float i, Vector3 const& col, float rad);
	void SetLightRadius(float rad);
	float GetLightRadius();
private:
	float radius;
};