#include "stdafx.h"
#include "Component.h"

#include "GameObject.h"

Component::Component()
	: Base()
	, Owner(nullptr)
	, mOrderFlag(0)
{
}

bool Component::operator<(const Component& other)
{
	return mOrderFlag < other.mOrderFlag;;
}

void Component::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	if (Owner != nullptr)
	{
		return;
	}

	Owner = owner;
	Owner->AddRefCount();
}

void Component::destroy()
{
	ReleaseBase(Owner);
}