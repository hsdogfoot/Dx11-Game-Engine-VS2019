#pragma once

#include <Windows.h>
#include <string>
#include <cstdint>
#include <memory>

#include <list>
#include <map>
#include <unordered_map>
#include <vector>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dinput.h>

using namespace DirectX;

#include "EngineUtility.h"
#include "ErrorLogger.h"
#include "StringConverter.h"