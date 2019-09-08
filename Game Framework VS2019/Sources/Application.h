#pragma once
#include "Base.h"

class RenderWindow;
class Graphics;

class Application abstract : public Base
{
public:
	Application();
	virtual ~Application() override = default;

	LRESULT WindowProc(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void destroy() override;

protected:
	RenderWindow* mRenderWindow;
	Graphics* mGraphics;
};