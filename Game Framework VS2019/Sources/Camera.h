#pragma once
#include "GameObject.h"

class CameraComponent;

class Camera : public GameObject
{
public:
	static Camera* Create();

	void SetCameraComponent(CameraComponent* cameraComponent);

	XMMATRIX GetView() const;
	XMMATRIX GetProjection() const;
	void SetTarget(GameObject* object);

	inline void SetEye(XMFLOAT3 eye);
	inline void AdjustEye(XMFLOAT3 eye);
	inline void SetRotation(XMFLOAT3 rotation);
	inline void AdjustRotation(XMFLOAT3 rotation);

	virtual void PreRelease() override;

protected:
	Camera();
	virtual ~Camera() override = default;

	virtual void destroy() override;

protected:
	CameraComponent* mCameraComponent;
};

inline void Camera::SetEye(XMFLOAT3 eye)
{
	mCameraComponent->SetEye(eye);
}

inline void Camera::AdjustEye(XMFLOAT3 eye)
{
	mCameraComponent->AdjustEye(eye);
}

inline void Camera::SetRotation(XMFLOAT3 rotation)
{
	mCameraComponent->SetRotation(rotation);
}

inline void Camera::AdjustRotation(XMFLOAT3 rotation)
{
	mCameraComponent->AdjustRotation(rotation);
}