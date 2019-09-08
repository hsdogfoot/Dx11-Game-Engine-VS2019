#pragma once
#include "Base.h"

// Initialize && LoadDefaultComponents && RemovePrototype delete

#include "Components.h"

class ComponentFactory final : public Base
{
public:
	static ComponentFactory* GetInstance();
	static uint32_t DestroyInstance();

	bool Initialize();

	bool RegisterPrototype(const WCHAR* tag, Component* prototype);
	void RemovePrototype(const WCHAR* tag);

	template <typename T, typename... Args>
	Component* GetCloneOrNull(const WCHAR* tag, Args... arguments) const;

private:
	ComponentFactory();
	virtual ~ComponentFactory() override = default;

	bool registerDefaultPrototypes();

	virtual void destroy() override;

private:
	static ComponentFactory* mInstance;

	std::unordered_map<const WCHAR*, Component*> mPrototypes;
};

template<typename T, typename ...Args>
Component* ComponentFactory::GetCloneOrNull(const WCHAR* tag, Args... arguments) const
{
	auto iter = mPrototypes.find(tag);
	if (iter == mPrototypes.end())
	{
		ErrorLog("Prototype find failed.");

		return nullptr;
	}

	return static_cast<T*>(iter->second)->CloneOrNull(arguments...);
}
