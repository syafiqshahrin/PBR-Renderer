#include "Transform.h"
#include "Debug.h"
Transform::Transform()
{
	position = Vector3::zero();
	rotation = Vector3::zero();
	scale = Vector3::one();
	parent = nullptr;
}

Transform::Transform(Transform* par)
{
	parent = par;
}

Transform::Transform(Vector3 pos, Vector3 rot, Vector3 s, Transform *par)
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
	Matrix4x4 R = Matrix4x4::GetRotationMatrix(rotation);
	Matrix4x4 T = Matrix4x4::GetTranslationMatrix(position);


	if (parent == nullptr)
	{
		ModelMatrix = T * (R * S);
	}
	else
	{
		ModelMatrix = S * R * T * parent->GetModelMatrix();
	}
	/*
	float m[16];
	ModelMatrix.GetMatrixFloatArray(m);
	for (int i = 0; i < 4; i++)
	{
		DEBUG(m[(i * 4)] << "," <<  m[(i * 4) + 1] << "," << m[(i * 4) + 2] << "," << m[(i * 4) + 3]);
	}
	*/
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
