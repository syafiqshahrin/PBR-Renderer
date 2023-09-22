#include "Transform.h"

Transform::Transform()
{

}

Transform::Transform(Vector3 pos, Vector3 rot, Vector3 s, Transform *par = nullptr)
{
	position = pos;
	rotation = rot;
	scale = s;
	parent = par;
}

void Transform::SetPosition(Vector3 pos)
{
	position = pos;
}

void Transform::SetScale(Vector3 s)
{
	scale = s;
}

void Transform::SetRotation(Vector3 rot)
{
	rotation = rot;

}

Vector3 Transform::GetPosition()
{
	return position;
}

Vector3 Transform::GetRotation()
{
	return rotation;
}

Vector3 Transform::GetScale()
{
	return scale;
}

Vector3 Transform::forward()
{
	Vector3 f3 = Vector3::forward();
	Vector4 f4 = ModelMatrix * f3.GetVec4(false);
	return f4.GetVec3().GetNormalize();
}

Vector3 Transform::right()
{
	Vector3 r3 = Vector3::right();
	Vector4 r4 = ModelMatrix * r3.GetVec4(false);
	return r4.GetVec3().GetNormalize();
}

Vector3 Transform::up()
{
	Vector3 u3 = Vector3::up();
	Vector4 u4 = ModelMatrix * u3.GetVec4(false);
	return u4.GetVec3().GetNormalize();
}

Matrix4x4 Transform::GetModelMatrix()
{
	return ModelMatrix;
}

void Transform::UpdateMatrix()
{
	Matrix4x4 S = Matrix4x4::GetScalingMatrix(scale);
	Matrix4x4 R = Matrix4x4::GetScalingMatrix(rotation);
	Matrix4x4 T = Matrix4x4::GetScalingMatrix(position);
	if(parent != nullptr)
		ModelMatrix = S * R * T;
	ModelMatrix = S * R * T * parent->GetModelMatrix();
}

Vector3 Transform::TransformPoint(Vector3 pos)
{
	UpdateMatrix();
	Vector3 u3 = Vector3::up();
	Vector4 u4 = ModelMatrix * u3.GetVec4(false);
	return u4.GetVec3().GetNormalize();
}

Vector3 Transform::TransformDirection(Vector3 dir)
{
	return Vector3();
}

void Transform::SetParent(Transform *par)
{
	parent = par;
}
