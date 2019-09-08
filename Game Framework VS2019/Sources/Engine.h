#pragma once
#include "Application.h"

class Engine final : public Application
{
public:
	static Engine* CreateOrNull(HINSTANCE hInstance, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight);

	void RunEngine();

private:
	Engine();
	virtual ~Engine() override = default;

	bool initialize(HINSTANCE hInstance, std::string windowTitle, std::string windowClassName, int windowWidth, int windowHeight);
	bool loadComponents() const;
	bool loadResources() const;
	bool loadActors() const;
	bool loadCameras() const;

	bool processMessages();
	void update();
	void renderFrame();

	virtual void destroy() override;
};