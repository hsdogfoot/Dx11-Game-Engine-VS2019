#include "stdafx.h"
#include "PickingManager.h"

#include "Actor.h"
#include "CameraManager.h"
#include "Input.h"
#include "RenderManager.h"

PickingManager* PickingManager::mInstance = nullptr;

PickingManager* PickingManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new PickingManager();
	}

	return mInstance;
}

uint32_t PickingManager::DestroyInstance()
{
	if (mInstance == nullptr)
	{
		return 0;
	}

	uint32_t refCount = mInstance->Release();
	if (refCount == 0)
	{
		mInstance = nullptr;
	}

	return refCount;
}

void PickingManager::Initialize(int screenWidth, int screenHeight)
{
	mScreenWidth = static_cast<float>(screenWidth);
	mScreenHeight = static_cast<float>(screenHeight);
}

void PickingManager::Pick()
{
	float mouseX = static_cast<float>(Input::GetInstance()->GetMouseX());
	float mouseY = static_cast<float>(Input::GetInstance()->GetMouseY());

	XMVECTOR a = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);
	XMVECTOR b = XMVectorSet(mouseX, mouseY, 1.0f, 1.0f);

	mouseX = 2.0f * mouseX / mScreenWidth - 1.0f;
	mouseY = 2.0f * mouseY / mScreenHeight - 1.0f;
	mouseY *= -1.0f;

	XMFLOAT3X3 proj;
	XMStoreFloat3x3(&proj, CameraManager::GetInstance()->MainCamera->GetProjection());

	XMVECTOR point = XMVectorSet(mouseX / proj._11, mouseY / proj._22, 0.0f, 1.0f);
	XMMATRIX projectionInverse = XMMatrixInverse(nullptr, CameraManager::GetInstance()->MainCamera->GetProjection());
	XMMATRIX viewInverse = XMMatrixInverse(nullptr, CameraManager::GetInstance()->MainCamera->GetView());
	//point = XMVector3TransformCoord(point, projectionInverse);
	point = XMVector3TransformCoord(point, viewInverse);
	a = XMVector3TransformCoord(a, projectionInverse);
	a = XMVector3TransformCoord(a, viewInverse);
	a = a - b;
	a = XMVector3Normalize(a);

	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, CameraManager::GetInstance()->MainCamera->GetView());
	XMVECTOR direction = XMVectorSet(view._31, view._32, view._33, 1.0f);
	direction = XMVector3Normalize(direction);

	XMVECTOR localVertices[] = 
	{
		XMVectorSet(-0.5f, -0.5f, 0.0f, 0.0f),
		XMVectorSet(-0.5f, +0.5f, 0.0f, 0.0f),
		XMVectorSet(+0.5f, +0.5f, 0.0f, 0.0f),
		XMVectorSet(+0.5f, -0.5f, 0.0f, 0.0f),
	};

	XMVECTOR worldVertices[] =
	{
		XMVectorZero(),
		XMVectorZero(),
		XMVectorZero(),
		XMVectorZero(),
	};

	XMMATRIX world;
	XMVECTOR planeNormal;
	XMFLOAT4 planeVector;
	float dot;
	float u;
	XMVECTOR meetPoint;
	for (auto actor : RenderManager::GetInstance()->Actors)
	{
		world = static_cast<Transform*>(actor->GetComponent(L"Transform"))->World;

		for (int i = 0; i < ARRAYSIZE(worldVertices); ++i)
		{
			worldVertices[i] = XMVector3TransformCoord(localVertices[i], world);
		}

		planeNormal = XMVector3Cross(worldVertices[1] - worldVertices[0], worldVertices[2] - worldVertices[0]);
		planeNormal = XMVector3Normalize(planeNormal);
		
		planeVector.x = XMVectorGetX(planeNormal);
		planeVector.y = XMVectorGetY(planeNormal);
		planeVector.z = XMVectorGetZ(planeNormal);

		planeVector.w = planeVector.x * XMVectorGetX(worldVertices[3]) + planeVector.y * XMVectorGetY(worldVertices[3]) + planeVector.z * XMVectorGetZ(worldVertices[3]);
		planeVector.w *= -1.0f;

		dot = XMVectorGetX(XMVector3Dot(planeNormal, direction));
		if (dot > 0)
		{
			continue;
		}

		// u = N * (P3 - P1) / N * (P2 - P1) P1, P2 : 직선 위의 점 P3 : 평면 위의 점 N : 노말
		u = XMVectorGetX(XMVector3Dot(planeNormal, worldVertices[3] - point)) / XMVectorGetX(XMVector3Dot(planeNormal, direction));
		meetPoint = point + u * a;

		if (isInRect(planeNormal, meetPoint, worldVertices))
		{
			ReleaseBase(SelectedActor);
			SelectedActor = actor;
			actor->AddRefCount();

			break;
		}
	}
}

PickingManager::PickingManager()
	: Base()
	, SelectedActor(nullptr)
	, mScreenWidth(0.0f)
	, mScreenHeight(0.0f)
{
}

bool PickingManager::isInRect(XMVECTOR planeNormal, XMVECTOR planePoint, XMVECTOR* vertices)
{
	XMVECTOR side;
	XMVECTOR cross;
	float dot;

	for (int i = 0; i < 4; ++i)
	{
		side = vertices[(i + 1) % 4] - vertices[i];
		cross = XMVector3Cross(side, planeNormal);
		dot = XMVectorGetX(XMVector3Dot(cross, planePoint - vertices[i]));
		if (dot > 0)
		{
			return false;
		}
	}

	return true;
}

void PickingManager::destroy()
{
	ReleaseBase(SelectedActor);
}
