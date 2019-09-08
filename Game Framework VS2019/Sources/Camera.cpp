#include "stdafx.h"
#include "Camera.h"

#include "CameraComponent.h"

Camera* Camera::Create()
{
	return new Camera();
}

void Camera::SetCameraComponent(CameraComponent* cameraComponent)
{
	Assert(cameraComponent != nullptr);

	if (mCameraComponent != nullptr)
	{
		ErrorLog("CameraComponent access error.");

		return;
	}

	mCameraComponent = cameraComponent;
	mCameraComponent->AddRefCount();

	IsInvalid = false;
}

XMMATRIX Camera::GetView() const
{
	return mCameraComponent->View;
}

XMMATRIX Camera::GetProjection() const
{
	return mCameraComponent->Projection;
}

void Camera::SetTarget(GameObject* object)
{
}

void Camera::PreRelease()
{
	GameObject::PreRelease();

	mCameraComponent->Release();
	mCameraComponent = nullptr;
}

Camera::Camera()
	: GameObject()
	, mCameraComponent(nullptr)
{
	IsInvalid = true;
}

void Camera::destroy()
{
	ReleaseBase(mCameraComponent);

	GameObject::destroy();
}
