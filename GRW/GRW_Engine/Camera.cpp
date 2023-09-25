#include "Camera.h"
#include "Transform.h"


Camera::Camera(Transform &tform)
{
	transform = &tform;
}

Camera::~Camera()
{

}

Matrix4x4 Camera::GetCameraViewMatrix()
{
	transform->UpdateMatrix();
	return transform->GetModelMatrix().GetInverse();
}

OrthoCamera::OrthoCamera(Transform &tform, Vector3 minBoundary, Vector3 maxBoundary, Vector2 ScreenDimensions) : Camera(tform)
{
	MinBoundary = minBoundary;
	MaxBoundary = maxBoundary;

	ProjectionMatrix = Matrix4x4::GetOrthoProjectionMatrix(MinBoundary, MaxBoundary, ScreenDimensions);
}

Matrix4x4 OrthoCamera::GetCameraProjectionMatrix()
{
	return ProjectionMatrix;
}
