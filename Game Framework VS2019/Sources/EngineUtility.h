#pragma once

#define __FILENAME_W__ (wcsrchr(_CRT_WIDE(__FILE__),'\\') + 1) 

#if defined(DEBUG) || defined(_DEBUG)
#define Assert(exp)	if (!(exp))																										\
					{																												\
						wchar_t message[256];																						\
						wsprintf(message, L"\nFile\t\t: %s\nFunction\t\t: %s()\nLine\t\t: %d\nExpression\t: %s\n"					\
									, __FILENAME_W__, _CRT_WIDE(__FUNCTION__), __LINE__, _CRT_WIDE(#exp));							\
						MessageBoxEx(nullptr, message, L"Assertion Failed", MB_ICONERROR, 0);										\
						__asm { int 3 }																								\
					}
#else
#define Assert(exp)	( __assume(exp) )
#endif

#define ReleaseBase(base) if (base) { base->Release(); }
#define ReleaseCOM(com) if (com) { com->Release(); com = nullptr; }