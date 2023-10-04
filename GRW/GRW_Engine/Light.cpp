#include "Light.h"
#include "Transform.h"
#include "Vector.h"

Light::Light(Transform& tform)
{
	transform = &tform;
	color = Vector3::one();
	intensity = 1.0f;
}

Light::Light(Transform& tform, float i, Vector3 const& col)
{
	transform = &tform;
	color = col;
	intensity = i;
}

Light::~Light()
{
}

void Light::SetPosition(Vector3 const& pos)
{
	transform->SetPosition(pos);
}

void Light::SetIntensity(float const& i)
{
	intensity = i;
}

void Light::SetColor(Vector3 const& col)
{
	color = col;
}

Vector3 Light::GetPosition()
{
	return transform->GetPosition();
}

float Light::GetIntensity()
{
	return intensity;
}

Vector3 Light::GetColor()
{
	return color;
}

DirectLight::DirectLight(Transform& tform, float i, Vector3 const& col, Vector3 const& dir) : Light(tform, i, col)
{
	lightDirection = dir;
}

void DirectLight::SetLightDirection(Vector3 const& dir)
{
	lightDirection = dir;
}

Vector3 DirectLight::GetLightDirection()
{
	return lightDirection;
}

PointLight::PointLight(Transform& tform, float i, Vector3 const& col, float rad) : Light(tform, i, col)
{
	radius = rad;
}

void PointLight::SetLightRadius(float rad)
{
	radius = rad;
}

float PointLight::GetLightRadius()
{
	return radius;
}
