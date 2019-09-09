#include "stdafx.h"
#include "RenderManager.h"

#include "Actor.h"

RenderManager* RenderManager::mInstance = nullptr;

RenderManager* RenderManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new RenderManager();
	}

	return mInstance;
}

uint32_t RenderManager::DestroyInstance()
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

void RenderManager::RegisterActor(Actor* actor)
{
	Assert(actor != nullptr);

	Actors.push_back(actor);
	actor->AddRefCount();
}

void RenderManager::Render()
{
	for (auto actor : Actors)
	{
		actor->Render();
		actor->Release();
	}

	Actors.clear();
}

RenderManager::RenderManager()
	: Base()
{
}

void RenderManager::destroy()
{
	for (auto actor : Actors)
	{
		actor->Release();
	}
}

