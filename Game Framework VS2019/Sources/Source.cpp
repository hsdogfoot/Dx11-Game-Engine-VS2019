#include "stdafx.h"

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "Engine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Engine* engine = Engine::CreateOrNull(hInstance, "Dx11 Game Engine VS2019", "Dx11 Game Engine VS2019", 800, 600);
	if (engine == nullptr)
	{
		return 0;
	}

	engine->RunEngine();

	return 0;
}