#include "stdafx.h"
#include "Input.h"

Input* Input::mInstance = nullptr;

Input* Input::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new Input();
	}

	return mInstance;
}

uint32_t Input::DestroyInstance()
{
	if (mInstance == nullptr)
	{
		return 0;
	}

	uint32_t refCount = mInstance->Release();
	if (refCount == 0)
	{
		mInstance = nullptr;
	}

	return refCount;
}

bool Input::Initialize(HINSTANCE hInstance, HWND hWND, int screenWidth, int screenHeight)
{
	Assert(hInstance != nullptr);
	Assert(hWND != nullptr);

	mScreenWidth = screenWidth;
	mScreenHeight = screenHeight;

	HRESULT hResult = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8W, reinterpret_cast<void**>(&mDirectInput), nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	if (!initializeKeyboard(hWND))
	{
		return false;
	}

	if (!initializeMouse(hWND))
	{
		return false;
	}

	return true;
}

void Input::Update()
{
	ReadKeyboard();
	ReadMouse();

	for (int i = 0; i < 256; ++i)
	{
		mPreviousKeyboardState[i] = mCurrentKeyboardState[i];

		if (mKeyboardState[i] & 0x80)
		{
			mCurrentKeyboardState[i] = true;
		}
		else
		{
			mCurrentKeyboardState[i] = false;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		mPreviousMouseState[i] = mCurrentMouseState[i];

		if (mMouseState.rgbButtons[i] & 0x80)
		{
			mCurrentMouseState[i] = true;
		}
		else
		{
			mCurrentMouseState[i] = false;
		}
	}
}

Input::Input()
	: Base()
	, mDirectInput(nullptr)
	, mKeyboard(nullptr)
	, mKeyboardState()
	, mCurrentKeyboardState()
	, mPreviousKeyboardState()
	, mMouse(nullptr)
	, mMouseState()
	, mCurrentMouseState()
	, mPreviousMouseState()
	, mMouseX(0)
	, mMouseY(0)
	, mScreenWidth(0)
	, mScreenHeight(0)
{
}

bool Input::initializeKeyboard(HWND hWND)
{
	HRESULT hResult = mDirectInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mKeyboard->SetCooperativeLevel(hWND, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mKeyboard->Acquire();
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

bool Input::initializeMouse(HWND hWND)
{
	HRESULT hResult = mDirectInput->CreateDevice(GUID_SysMouse, &mMouse, nullptr);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mMouse->SetCooperativeLevel(hWND, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	hResult = mMouse->Acquire();
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

bool Input::ReadKeyboard()
{
	HRESULT hResult = mKeyboard->GetDeviceState(sizeof(mKeyboardState), reinterpret_cast<LPVOID>(&mKeyboardState));
	if (FAILED(hResult))
	{
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED)
		{
			mKeyboard->Acquire();
		}

		return false;
	}

	return true;
}

bool Input::ReadMouse()
{
	HRESULT hResult = mMouse->GetDeviceState(sizeof(DIMOUSESTATE), reinterpret_cast<LPVOID>(&mMouseState));
	if (FAILED(hResult))
	{
		if (hResult == DIERR_INPUTLOST || hResult == DIERR_NOTACQUIRED)
		{
			mMouse->Acquire();
		}

		return false;
	}

	mMouseX += mMouseState.lX;
	mMouseY += mMouseState.lY;

	if (mMouseX < 0)
	{
		mMouseX = 0;
	}

	if (mMouseY < 0)
	{
		mMouseY = 0;
	}

	if (mMouseX > mScreenWidth)
	{
		mMouseX = mScreenWidth;
	}

	if (mMouseY > mScreenHeight)
	{
		mMouseY = mScreenHeight;
	}

	return true;
}

void Input::destroy()
{
	if (mKeyboard != nullptr)
	{
		mKeyboard->Unacquire();
		mKeyboard->Release();
	}

	if (mMouse != nullptr)
	{
		mMouse->Unacquire();
		mMouse->Release();
	}

	ReleaseCOM(mDirectInput);
}
