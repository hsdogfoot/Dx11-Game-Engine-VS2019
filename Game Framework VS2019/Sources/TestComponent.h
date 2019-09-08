#pragma once
#include "Component.h"

#include "CBufferTypes.h"

class TestComponent final : public Component
{
public:
	static TestComponent* Create();
	TestComponent* CloneOrNull();

	virtual uint8_t Update() override;

private:
	TestComponent();
	explicit TestComponent(const TestComponent& other);
	virtual ~TestComponent() override = default;

	bool initializeClone();

	virtual void destroy() override;

private:
	CB_PS_COLOR mData;
	bool mbToggle;
};