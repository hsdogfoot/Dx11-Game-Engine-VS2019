#include "stdafx.h"
#include "CameraComponent.h"

#include "Camera.h"

const XMVECTOR CameraComponent::DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
const XMVECTOR CameraComponent::DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
const XMVECTOR CameraComponent::DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

CameraComponent* CameraComponent::Create()
{
	return new CameraComponent();
}

CameraComponent* CameraComponent::CloneOrNull(eProjectionType projectionType, XMFLOAT3 eye, XMFLOAT3 lookAt, float fovAngle, float screenWidth, float screenHeight, float nearZ, float farZ)
{
	CameraComponent* instance = new CameraComponent(*this);
	if (!instance->initializeClone(projectionType, eye, lookAt, fovAngle, screenWidth, screenHeight, nearZ, farZ))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

void CameraComponent::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);

	static_cast<Camera*>(Owner)->SetCameraComponent(this);
}

uint8_t CameraComponent::Update()
{
	updateView();

	return 0;
}

void CameraComponent::SetLookAtPos(XMVECTOR lookAt)
{
	XMStoreFloat3(&LookAt, lookAt);

	if (Eye.x == LookAt.x && Eye.y == LookAt.y && Eye.z == LookAt.z)
	{
		return;
	}

	LookAt.x = Eye.x - LookAt.x;
	LookAt.y = Eye.y - LookAt.y;
	LookAt.z = Eye.z - LookAt.z;

	float pitch = 0.0f;
	if (LookAt.y != 0.0f)
	{
		float distance = sqrtf(LookAt.x * LookAt.x + LookAt.z * LookAt.z);
		if (distance != 0.0f)
		{
			pitch = atanf(LookAt.y / distance);
		}
		else if (LookAt.y > 0.0f)
		{
			pitch = XM_PI / 2.0f;
		}
		else
		{
			pitch = -XM_PI / 2.0f;
		}
	}

	float yaw = 0.0f;
	if (LookAt.x != 0.0f)
	{
		if (LookAt.z != 0.0f)
		{
			yaw = atanf(LookAt.x / LookAt.z);
		}
		else if (LookAt.x > 0.0f)
		{
			yaw = XM_PI / 2.0f;
		}
		else
		{
			yaw = -XM_PI / 2.0f;
		}
	}

	if (LookAt.z > 0.0f)
	{
		yaw += XM_PI;
	}

	SetRotation(XMFLOAT3(pitch, yaw, 0.0f));
}

void CameraComponent::SetProjection(float fovAngle, float screenWidth, float screenHeight, float nearZ, float farZ)
{
	switch (mProjectionType)
	{
	case eProjectionType::Perspective:
		Projection = XMMatrixPerspectiveFovLH(fovAngle, screenWidth / screenHeight, nearZ, farZ);
		break;
	case eProjectionType::Orthographic:
		Projection = XMMatrixOrthographicLH(screenWidth, screenHeight, nearZ, farZ);
		break;
	default:
		Assert(false);
		break;
	}
}

CameraComponent::CameraComponent()
	: Component()
	, mProjectionType(eProjectionType::Perspective)
	, Eye(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, LookAt(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, Rotation(XMFLOAT3(0.0f, 0.0f, 0.0f))
	, View(XMMatrixIdentity())
	, Projection(XMMatrixIdentity())
{
}

CameraComponent::CameraComponent(const CameraComponent& other)
	: Component()
	, mProjectionType(other.mProjectionType)
	, Eye(other.Eye)
	, LookAt(other.LookAt)
	, Rotation(other.Rotation)
	, View(other.View)
	, Projection(other.Projection)
{
}

void* CameraComponent::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void CameraComponent::operator delete(void* memory)
{
	_aligned_free(memory);
}

bool CameraComponent::initializeClone(eProjectionType projectionType, XMFLOAT3 eye, XMFLOAT3 lookAt, float fovAngle, float screenWidth, float screenHeight, float nearZ, float farZ)
{
	mProjectionType = projectionType;

	Eye = eye;
	LookAt = lookAt;

	if (Eye.x == LookAt.x && Eye.y == LookAt.y && Eye.z == LookAt.z)
	{
		XMVECTOR lookAtPos = XMLoadFloat3(&Eye) + DEFAULT_FORWARD_VECTOR;
		XMStoreFloat3(&LookAt, lookAtPos);
	}

	SetLookAtPos(XMLoadFloat3(&LookAt));
	SetProjection(fovAngle, screenWidth, screenHeight, nearZ, farZ);

	return true;
}

void CameraComponent::updateView()
{
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&Rotation));
	XMVECTOR lookAt = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, rotation);
	XMVECTOR eye = XMLoadFloat3(&Eye);
	lookAt += eye;

	View = XMMatrixLookAtLH(eye, lookAt, DEFAULT_UP_VECTOR);
}

void CameraComponent::destroy()
{
	Component::destroy();
}
