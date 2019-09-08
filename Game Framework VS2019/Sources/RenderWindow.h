#pragma once
#include "Base.h"

class Application;

class RenderWindow final : public Base
{
public:
	static RenderWindow* CreateOrNull(HINSTANCE hInstance, std::string title, std::string className, int width, int height, Application* application);

	bool ProcessMessages();

private:
	RenderWindow() = delete;
	explicit RenderWindow(HINSTANCE hInstance, std::string title, std::string className, int width, int height);
	virtual ~RenderWindow() override = default;

	bool initialize(Application* application);
	bool registerWindowClass();
	bool createWindow(Application* application);

	virtual void destroy() override;

public:
	HWND WindowHandle;

	int Width;
	int Height;

private:
	HINSTANCE mHInstance;

	std::string mTitle;
	std::string mClassName;

	std::wstring mTitleWide;
	std::wstring mClassNameWide;
};