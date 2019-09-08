#include "stdafx.h"
#include "Renderer.h"

#include "Actor.h"

Renderer::Renderer()
	: Component()
	, mDevice(nullptr)
	, mDeviceContext(nullptr)
{
}

void Renderer::SetOwner(GameObject* owner)
{
	Assert(owner != nullptr);

	Component::SetOwner(owner);

	static_cast<Actor*>(Owner)->SetRenderer(this);
}

uint8_t Renderer::Update()
{
	return 0;
}

void Renderer::destroy()
{
	ReleaseCOM(mDevice);
	ReleaseCOM(mDeviceContext);

	Component::destroy();
}
