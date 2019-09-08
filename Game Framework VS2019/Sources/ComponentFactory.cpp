#include "stdafx.h"
#include "ComponentFactory.h"

ComponentFactory* ComponentFactory::mInstance = nullptr;

ComponentFactory* ComponentFactory::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ComponentFactory();
	}

	return mInstance;
}

uint32_t ComponentFactory::DestroyInstance()
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

bool ComponentFactory::Initialize()
{
	if (!registerDefaultPrototypes())
	{
		ErrorLog("Register default component prototypes faield.");

		return false;
	}

	return true;
}

bool ComponentFactory::RegisterPrototype(const WCHAR* tag, Component* prototype)
{
	Assert(tag != nullptr);
	Assert(prototype != nullptr);

	if (mPrototypes.find(tag) != mPrototypes.end())
	{
		return false;
	}

	mPrototypes.insert(std::pair<const WCHAR*, Component*>(tag, prototype));

	return true;
}

void ComponentFactory::RemovePrototype(const WCHAR* tag)
{
	auto iter = mPrototypes.find(tag);
	if (iter == mPrototypes.end())
	{
		return;
	}

	iter->second->Release();
	mPrototypes.erase(iter);
}

ComponentFactory::ComponentFactory()
	: Base()
{
}

bool ComponentFactory::registerDefaultPrototypes()
{
	bool bResult = true;
	bResult &= RegisterPrototype(L"CameraComponent", CameraComponent::Create());
	bResult &= RegisterPrototype(L"Transform", Transform::Create());
	bResult &= RegisterPrototype(L"Renderer2D", Renderer2D::Create());

	return bResult;
}

void ComponentFactory::destroy()
{
	for (auto pair : mPrototypes)
	{
		pair.second->Release();
	}
}