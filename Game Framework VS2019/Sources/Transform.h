#pragma once
#include "Component.h"

class Transform final : public Component
{
public:
	static Transform* Create();
	Transform* CloneOrNull(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position);

	virtual uint8_t Update() override;

private:
	Transform();
	explicit Transform(const Transform& other);
	virtual ~Transform() override = default;

	void* operator new(size_t size);
	void operator delete(void* memory);

	bool initializeClone(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position);

	virtual void destroy() override;

public:
	XMVECTOR Scale;
	XMVECTOR Rotation;
	XMVECTOR Position;
	XMVECTOR Revolution;
	XMMATRIX Parent;

	XMMATRIX World;
};