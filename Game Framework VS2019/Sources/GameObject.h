#pragma once
#include "Base.h"

#include "ComponentFactory.h"

class Component;

class GameObject : public Base
{
public:
	static GameObject* Create();

	template <typename T, typename... Args>
	bool EquipComponent(const WCHAR* tag, const WCHAR* protoKey, Args... arguments);

	void RemoveComponent(const WCHAR* tag);
	Component* GetComponent(const WCHAR* tag);

	virtual uint8_t Update();

	virtual void PreRelease();

protected:
	GameObject();
	virtual ~GameObject() override = default;

	virtual void destroy() override;

public:
	bool IsInvalid;

protected:
	std::unordered_map<const WCHAR*, Component*> mComponents;
	uint32_t mComponentID;
};

template<typename T, typename ...Args>
bool GameObject::EquipComponent(const WCHAR* tag, const WCHAR* protoKey, Args ...arguments)
{
	if (mComponents.find(tag) != mComponents.end())
	{
		ErrorLog("Tag already used.");

		return false;
	}

	Component* clone = ComponentFactory::GetInstance()->GetCloneOrNull<T>(protoKey, arguments...);
	if (clone == nullptr)
	{
		return false;
	}

	mComponents.insert(std::pair<const WCHAR*, Component*>(tag, clone));
	clone->SetOwner(this);
	clone->SetOrderFlag(++mComponentID);

	return true;
}