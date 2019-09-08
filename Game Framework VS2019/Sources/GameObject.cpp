#include "stdafx.h"
#include "GameObject.h"

#include <algorithm>
#include "Component.h"

GameObject* GameObject::Create()
{
	return new GameObject();
}

void GameObject::RemoveComponent(const WCHAR* tag)
{
	auto iter = mComponents.find(tag);
	if (iter == mComponents.end())
	{
		return;
	}

	iter->second->Release();
	mComponents.erase(iter);
}

Component* GameObject::GetComponent(const WCHAR* tag)
{
	auto iter = mComponents.find(tag);
	Assert(iter != mComponents.end());

	return iter->second;
}

uint8_t GameObject::Update()
{
	if (IsInvalid)
	{
		return 1;
	}

	std::vector<Component*> components;
	components.reserve(mComponents.size());
	for (auto pair : mComponents)
	{
		components.push_back(pair.second);
	}

	std::sort(components.begin(), components.end(), ComponentComparer());

	uint8_t exitCode = 0;
	for (auto component : components)
	{
		exitCode |= component->Update();
	}

	return exitCode;
}

void GameObject::PreRelease()
{
	IsInvalid = true;

	for (auto pair : mComponents)
	{
		pair.second->Release();
	}

	mComponents.clear();
}

GameObject::GameObject()
	: Base()
	, IsInvalid(false)
	, mComponentID(0)
{
}

void GameObject::destroy()
{
	for (auto pair : mComponents)
	{
		pair.second->Release();
	}
}

