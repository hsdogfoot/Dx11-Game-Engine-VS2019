#include "stdafx.h"
#include "RenderWindow.h"

#include "Application.h"

LRESULT CALLBACK HandleMsgRedirect(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE)
	{
		DestroyWindow(hWND);

		return 0;
	}

	Application* const application = reinterpret_cast<Application*>(GetWindowLongPtr(hWND, GWLP_USERDATA));

	return application->WindowProc(hWND, uMsg, wParam, lParam);
}

LRESULT CALLBACK HandleMessageSetup(HWND hWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Application* application = reinterpret_cast<Application*>(pCreate->lpCreateParams);
		if (application == nullptr)
		{
			ErrorLog("Application pointer was nullptr.");

			exit(-1);
		}

		SetWindowLongPtr(hWND, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
		SetWindowLongPtr(hWND, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));

		return application->WindowProc(hWND, uMsg, wParam, lParam);
	}

	return DefWindowProc(hWND, uMsg, wParam, lParam);
}

RenderWindow* RenderWindow::CreateOrNull(HINSTANCE hInstance, std::string title, std::string className, int width, int height, Application* application)
{
	Assert(hInstance != nullptr);
	Assert(application != nullptr);

	RenderWindow* instance = new RenderWindow(hInstance, title, className, width, height);
	if (!instance->initialize(application))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

bool RenderWindow::ProcessMessages()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (PeekMessage(&msg, WindowHandle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL)
	{
		if (!IsWindow(WindowHandle))
		{
			WindowHandle = nullptr;
			UnregisterClass(mClassNameWide.c_str(), mHInstance);

			return false;
		}
	}

	return true;
}

RenderWindow::RenderWindow(HINSTANCE hInstance, std::string title, std::string className, int width, int height)
	: Base()
	, WindowHandle(nullptr)
	, Width(width)
	, Height(height)
	, mHInstance(hInstance)
	, mTitle(title)
	, mClassName(className)
{
	mTitleWide = StringConverter::ConvertStringToWide(mTitle);
	mClassNameWide = StringConverter::ConvertStringToWide(mClassName);
}

bool RenderWindow::initialize(Application* application)
{
	if (!registerWindowClass())
	{
		return false;
	}

	if (!createWindow(application))
	{
		return false;
	}

	ShowWindow(WindowHandle, SW_SHOW);
	SetForegroundWindow(WindowHandle);
	SetFocus(WindowHandle);

	return true;
}

bool RenderWindow::registerWindowClass()
{
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHInstance;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = mClassNameWide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		ErrorLog("RegisterClassEx failed.");

		return false;
	}

	return true;
}
bool RenderWindow::createWindow(Application* application)
{
	RECT windowRect;
	windowRect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - Width / 2;
	windowRect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - Height / 2;
	windowRect.right = windowRect.left + Width;
	windowRect.bottom = windowRect.top + Height;
	DWORD windowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_THICKFRAME;
	AdjustWindowRect(&windowRect, windowStyle, false);

	WindowHandle = CreateWindowEx(0, mClassNameWide.c_str(), mTitleWide.c_str(), windowStyle, windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, nullptr, nullptr, mHInstance, application);
	if (WindowHandle == nullptr)
	{
		ErrorLog("CreateWindowEx failed.");

		return false;
	}

	return true;
}

void RenderWindow::destroy()
{
	if (WindowHandle != nullptr)
	{
		UnregisterClass(mClassNameWide.c_str(), mHInstance);
		DestroyWindow(WindowHandle);
	}
}