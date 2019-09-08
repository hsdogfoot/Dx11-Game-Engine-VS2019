#pragma once
#include "GameObject.h"

class Renderer;

class Actor : public GameObject
{
public:
	static Actor* Create();

	void SetRenderer(Renderer* renderer);

	void Render() const;

	virtual void PreRelease() override;

protected:
	Actor();
	virtual ~Actor() override = default;

	virtual void destroy() override;

protected:
	Renderer* mRenderer;
};