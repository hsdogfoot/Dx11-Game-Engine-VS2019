#pragma once
#include "Base.h"

class GameObject;

class Component abstract : public Base
{
public:
	Component();
	virtual ~Component() override = default;

	bool operator<(const Component& other);

	virtual void SetOwner(GameObject* owner);
	inline void SetOrderFlag(uint32_t flag);

	virtual uint8_t Update() = 0;

protected:
	virtual void destroy() override;

public:
	GameObject* Owner;

protected:
	uint32_t mOrderFlag;
};

class ComponentComparer
{
public:
	bool operator()(Component* left, Component* right)
	{
		return *left < *right;
	}
};

inline void Component::SetOrderFlag(uint32_t flag)
{
	mOrderFlag = flag;
}