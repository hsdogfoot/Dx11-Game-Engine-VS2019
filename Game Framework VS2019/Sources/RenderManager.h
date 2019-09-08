#pragma once
#include "Base.h"

class Actor;

class RenderManager final : public Base
{
public:
	static RenderManager* GetInstance();
	static uint32_t DestroyInstance();

	void RegisterActor(Actor* actor);

	void Render();

private:
	RenderManager();
	virtual ~RenderManager() override = default;

	virtual void destroy() override;

private:
	static RenderManager* mInstance;

	std::vector<Actor*> mActors;
};