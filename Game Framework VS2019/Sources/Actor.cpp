#include "stdafx.h"
#include "Actor.h"

#include "Renderer.h"

Actor* Actor::Create()
{
	return new Actor();
}

void Actor::SetRenderer(Renderer* renderer)
{
	Assert(renderer != nullptr);

	if (mRenderer != nullptr)
	{
		ErrorLog("Renderer access error.");

		return;
	}

	mRenderer = renderer;
	mRenderer->AddRefCount();

	IsInvalid = false;
}

void Actor::Render() const
{
	if (IsInvalid)
	{
		return;
	}

	mRenderer->Render();
}

void Actor::PreRelease()
{
	GameObject::PreRelease();

	mRenderer->Release();
	mRenderer = nullptr;
}

Actor::Actor()
	: GameObject()
	, mRenderer(nullptr)
{
	IsInvalid = true;
}

void Actor::destroy()
{
	ReleaseBase(mRenderer);

	GameObject::destroy();
}
