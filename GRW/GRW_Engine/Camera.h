#pragma once
#include "Vector.h"
#include "Matrix.h"

struct Transform;

class Camera
{
public:
	Camera(Transform &tform);
	~Camera();


	Matrix4x4 GetCameraViewMatrix();
	virtual Matrix4x4 GetCameraProjectionMatrix() = 0;

protected:
	Transform* transform;
};


class OrthoCamera : public Camera
{
public:
	OrthoCamera(Transform &tform, Vector3 minBoundary, Vector3 maxBoundary, Vector2 ScreenDimensions);
	Matrix4x4 GetCameraProjectionMatrix() override;
private:
	Vector3 MinBoundary;
	Vector3 MaxBoundary;
	Matrix4x4 ProjectionMatrix;
};