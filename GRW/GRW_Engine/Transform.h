#pragma once

#include "Vector.h"
#include "Matrix.h"

struct Transform
{
	Transform();
	Transform(Transform* par);
	Transform(Vector3 pos, Vector3 rot, Vector3 s, Transform * par = nullptr);

	void SetPosition(Vector3 pos);
	void SetScale(Vector3 s);
	void SetRotation(Vector3 rot);

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();

	Vector3 forward();
	Vector3 right();
	Vector3 up();

	Matrix4x4 GetModelMatrix();

	void UpdateMatrix();
	
	Vector3 TransformPoint(Vector3 pos);
	Vector3 InvTransformPoint(Vector3 pos);

	Vector3 TransformDirection(Vector3 dir);
	Vector3 InvTransformDirection(Vector3 dir);

	void SetParent(Transform *par);

private:
	Vector3 position;
	Vector3 scale;
	Vector3 rotation;
	Transform* parent;
	Matrix4x4 ModelMatrix;



};