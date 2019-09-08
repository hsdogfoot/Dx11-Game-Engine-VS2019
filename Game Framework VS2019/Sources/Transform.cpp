#include "stdafx.h"
#include "Transform.h"

Transform* Transform::Create()
{
	return new Transform();
}

Transform* Transform::CloneOrNull(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position)
{
	Transform* instance = new Transform(*this);
	if (!instance->initializeClone(scale, rotation, position))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

uint8_t Transform::Update()
{
	World = XMMatrixScalingFromVector(Scale) 
		* XMMatrixRotationRollPitchYawFromVector(Rotation) 
		* XMMatrixTranslationFromVector(Position) 
		* XMMatrixRotationRollPitchYawFromVector(Revolution) 
		* Parent;

	return 0;
}

Transform::Transform()
	: Component()
	, Scale(XMVectorZero())
	, Rotation(XMVectorZero())
	, Position(XMVectorZero())
	, Revolution(XMVectorZero())
	, Parent(XMMatrixIdentity())
	, World(XMMatrixIdentity())
{
}

Transform::Transform(const Transform& other)
	: Component()
	, Scale(other.Scale)
	, Rotation(other.Rotation)
	, Position(other.Position)
	, Revolution(other.Revolution)
	, Parent(other.Parent)
	, World(other.World)
{
}

void* Transform::operator new(size_t size)
{
	return _aligned_malloc(size, 16);
}

void Transform::operator delete(void* memory)
{
	_aligned_free(memory);
}

bool Transform::initializeClone(XMVECTOR scale, XMVECTOR rotation, XMVECTOR position)
{
	Scale = scale;
	Rotation = rotation;
	Position = position;

	return true;
}

void Transform::destroy()
{
	Component::destroy();
}
