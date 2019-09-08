#include "stdafx.h"
#include "Application.h"

#include "Graphics.h"
#include "RenderWindow.h"

Application::Application()
	: Base()
	, mRenderWindow(nullptr)
	, mGraphics(nullptr)
{
}

LRESULT Application::WindowProc(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWND, uMsg, wParam, lParam);
}

void Application::destroy()
{
	ReleaseBase(mRenderWindow);
	ReleaseBase(mGraphics);
}
