#include "stdafx.h"
#include "TestComponent.h"

#include "Actor.h"
#include "Resources.h"

TestComponent* TestComponent::Create()
{
	return new TestComponent();
}

TestComponent* TestComponent::CloneOrNull()
{
	TestComponent* instance = new TestComponent(*this);
	if (!instance->initializeClone())
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

uint8_t TestComponent::Update()
{
	if (mbToggle)
	{
		mData.red += 0.01f;
		mData.green += 0.01f;
		mData.blue += 0.01f;

		if (mData.red > 1.0f)
		{
			mbToggle = false;
		}
	}
	else
	{
		mData.red -= 0.01f;
		mData.green -= 0.01f;
		mData.blue -= 0.01f;

		if (mData.red < 0.0f)
		{
			mbToggle = true;
		}
	}

	PixelShader* shader = static_cast<Renderer2D*>(Owner->GetComponent(L"Renderer"))->PShader;
	static_cast<TPixelShader<CB_PS_COLOR>*>(shader)->SetCBufferData(&mData);

	return 0;
}

TestComponent::TestComponent()
	: Component()
	, mData()
	, mbToggle(false)
{
}

TestComponent::TestComponent(const TestComponent& other)
	: Component()
	, mData(other.mData)
	, mbToggle(other.mbToggle)
{
}

bool TestComponent::initializeClone()
{
	return true;
}

void TestComponent::destroy()
{
	Component::destroy();
}
