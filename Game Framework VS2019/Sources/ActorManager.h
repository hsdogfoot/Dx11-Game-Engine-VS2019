#pragma once
#include "Base.h"

#include "Actor.h"

class ActorManager final : public Base
{
public:
	static ActorManager* GetInstance();
	static uint32_t DestroyInstance();

	void RegisterActor(Actor* actor);

	void Update();

private:
	ActorManager();
	virtual ~ActorManager() override = default;

	void readyRendering();

	virtual void destroy() override;

private:
	static ActorManager* mInstance;

	std::vector<Actor*> mActors;
};