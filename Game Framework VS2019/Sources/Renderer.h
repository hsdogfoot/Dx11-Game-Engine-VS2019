#pragma once
#include "Component.h"

class Renderer abstract : public Component
{
public:
	Renderer();
	virtual ~Renderer() override = default;

	virtual void SetOwner(GameObject* owner) override;

	virtual uint8_t Update() override;
	virtual void Render() const = 0;

protected:
	virtual void destroy() override;

protected:
	ID3D11Device* mDevice;
	ID3D11DeviceContext* mDeviceContext;
};