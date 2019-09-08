#pragma once
#include "Component.h"

class CameraComponent final : public Component
{
public:
	enum class eProjectionType { Perspective, Orthographic };

public:
	static CameraComponent* Create();
	CameraComponent* CloneOrNull(eProjectionType projectionType, XMFLOAT3 eye, XMFLOAT3 lookAt, float fovAngle, float screenWidth, float screenHeight, float nearZ, float farZ);

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;

	void SetLookAtPos(XMVECTOR lookAt);
	void SetProjection(float fovAngle, float screenWidth, float screenHeight, float nearZ, float farZ);

	inline void SetEye(XMFLOAT3 eye);
	inline void AdjustEye(XMFLOAT3 eye);

	inline void SetRotation(XMFLOAT3 rotation);
	inline void AdjustRotation(XMFLOAT3 rotation);

private:
	CameraComponent();
	explicit CameraComponent(const CameraComponent& other);
	virtual ~CameraComponent() override = default;

	void* operator new(size_t size);
	void operator delete(void* memory);

	bool initializeClone(eProjectionType projectionType, XMFLOAT3 eye, XMFLOAT3 lookAt, float fovAngle, float screenWidth, float screenHeight, float nearZ, float farZ);

	void updateView();

	virtual void destroy() override;

public:
	eProjectionType mProjectionType;

	XMFLOAT3 Eye;
	XMFLOAT3 LookAt;
	XMFLOAT3 Rotation;

	XMMATRIX View;
	XMMATRIX Projection;

	static const XMVECTOR DEFAULT_RIGHT_VECTOR;
	static const XMVECTOR DEFAULT_UP_VECTOR;
	static const XMVECTOR DEFAULT_FORWARD_VECTOR;
};

inline void CameraComponent::SetEye(XMFLOAT3 eye)
{
	Eye = eye;
}

inline void CameraComponent::AdjustEye(XMFLOAT3 eye)
{
	Eye.x += eye.x;
	Eye.y += eye.y;
	Eye.z += eye.z;
}

inline void CameraComponent::SetRotation(XMFLOAT3 rotation)
{
	Rotation = rotation;
}

inline void CameraComponent::AdjustRotation(XMFLOAT3 rotation)
{
	Rotation.x += rotation.x;
	Rotation.y += rotation.y;
	Rotation.z += rotation.z;
}
