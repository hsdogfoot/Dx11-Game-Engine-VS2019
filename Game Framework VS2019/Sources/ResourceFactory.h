#pragma once
#include "Base.h"

#include "Resources.h"

class ResourceFactory final : public Base
{
public:
	static ResourceFactory* GetInstance();
	static uint32_t DestroyInstance();

	bool RegisterPrototype(const WCHAR* tag, Resource* prototype);
	bool RemovePrototype(const WCHAR* tag);

	Resource* GetCloneOrNull(const WCHAR* tag) const;

private:
	ResourceFactory();
	virtual ~ResourceFactory() override = default;

	virtual void destroy() override;

private:
	static ResourceFactory* mInstance;

	std::unordered_map<const WCHAR*, Resource*> mPrototypes;
};