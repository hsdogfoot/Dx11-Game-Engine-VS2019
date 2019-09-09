#pragma once
#include "Base.h"

class Input final : public Base
{
public:
	enum class eMouseKeyType { Left, Right, Wheel };
	enum { KEYBOARD_SIZE = 256, MOUSE_SIZE = 3 };

public:
	static Input* GetInstance();
	static uint32_t DestroyInstance();

	bool Initialize(HINSTANCE hInstance, HWND hWND, int screenWidth, int screenHeight);

	inline bool IsKeyDown(unsigned char keyCode) const;
	inline bool IsKeyPressed(unsigned char keyCode) const;
	inline bool IsKeyUp(unsigned char keyCode) const;

	inline bool IsMouseDown(eMouseKeyType key) const;
	inline bool IsMousePressed(eMouseKeyType key) const;
	inline bool IsMouseUp(eMouseKeyType key) const;

	inline int GetMouseX() const;
	inline int GetMouseY() const;

	void Update();

private:
	Input();
	virtual ~Input() override = default;

	bool initializeKeyboard();
	bool initializeMouse();

	bool ReadKeyboard();
	bool ReadMouse();

	virtual void destroy() override;

private:
	static Input* mInstance;

	HWND mWindowHandle;

	IDirectInput8W* mDirectInput;

	IDirectInputDevice8W* mKeyboard;
	unsigned char mKeyboardState[KEYBOARD_SIZE];
	bool mCurrentKeyboardState[KEYBOARD_SIZE];
	bool mPreviousKeyboardState[KEYBOARD_SIZE];
	
	IDirectInputDevice8W* mMouse;
	DIMOUSESTATE mMouseState;
	bool mCurrentMouseState[MOUSE_SIZE];
	bool mPreviousMouseState[MOUSE_SIZE];
	POINT mMousePos;

	int mScreenWidth;
	int mScreenHeight;
};

inline bool Input::IsKeyDown(unsigned char keyCode) const
{
	return !mPreviousKeyboardState[keyCode] && mCurrentKeyboardState[keyCode];
}

inline bool Input::IsKeyPressed(unsigned char keyCode) const
{
	return mPreviousKeyboardState[keyCode] && mCurrentKeyboardState[keyCode];
}

inline bool Input::IsKeyUp(unsigned char keyCode) const
{
	return mPreviousKeyboardState[keyCode] && !mCurrentKeyboardState[keyCode];
}

inline bool Input::IsMouseDown(eMouseKeyType key) const
{
	return !mPreviousMouseState[static_cast<int>(key)] && mCurrentMouseState[static_cast<int>(key)];
}

inline bool Input::IsMousePressed(eMouseKeyType key) const
{
	return mPreviousMouseState[static_cast<int>(key)] && mCurrentMouseState[static_cast<int>(key)];
}

inline bool Input::IsMouseUp(eMouseKeyType key) const
{
	return mPreviousMouseState[static_cast<int>(key)] && !mCurrentMouseState[static_cast<int>(key)];
}

inline int Input::GetMouseX() const
{	
	return mMousePos.x;
}

inline int Input::GetMouseY() const
{
	return mMousePos.y;
}