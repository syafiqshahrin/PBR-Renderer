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

PerspectiveCamera::PerspectiveCamera(Transform& tform, float f, float nPlane, float fPlane, Vector2 sDimensions) : Camera(tform)
{
	FarPlane = fPlane;
	NearPlane = nPlane;
	fov = f;
	screenDimension = sDimensions;

	float aspectRatio = sDimensions.y / sDimensions.x;

	ProjectionMatrix = Matrix4x4::GetPerspectiveProjectionMatrix(fov, NearPlane, FarPlane, aspectRatio);

}

Matrix4x4 PerspectiveCamera::GetCameraProjectionMatrix()
{
	return ProjectionMatrix;
}
