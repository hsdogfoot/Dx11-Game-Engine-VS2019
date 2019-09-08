#include "stdafx.h"
#include "ResourceFactory.h"

ResourceFactory* ResourceFactory::mInstance = nullptr;

ResourceFactory* ResourceFactory::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new ResourceFactory();
	}

	return mInstance;
}

uint32_t ResourceFactory::DestroyInstance()
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

bool ResourceFactory::RegisterPrototype(const WCHAR* tag, Resource* prototype)
{
	if (prototype == nullptr)
	{
		return false;
	}

	if (mPrototypes.find(tag) != mPrototypes.end())
	{
		return false;
	}

	mPrototypes.insert(std::pair<const WCHAR*, Resource*>(tag, prototype));

	return true;
}

bool ResourceFactory::RemovePrototype(const WCHAR* tag)
{
	auto iter = mPrototypes.find(tag);
	if (iter == mPrototypes.end())
	{
		return false;
	}

	mPrototypes.erase(iter);

	return true;
}

Resource* ResourceFactory::GetCloneOrNull(const WCHAR* tag) const
{
	auto iter = mPrototypes.find(tag);
	if (iter == mPrototypes.end())
	{
		ErrorLog("Prototype find failed.");

		return nullptr;
	}

	return iter->second->Clone();
}

ResourceFactory::ResourceFactory()
	: Base()
{
}

void ResourceFactory::destroy()
{
	for (auto pair : mPrototypes)
	{
		pair.second->Release();
	}
}
