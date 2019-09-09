#pragma once
#include <DirectXMath.h>

using namespace DirectX;

struct CB_VS_EMPTY
{
	char padding;
};

struct CB_PS_EMPTY
{
	char padding;
};

struct CB_VS_WVP
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;
};

struct CB_PS_COLOR
{
	float red;
	float green;
	float blue;
};

struct CB_PS_OFFSET
{
	float xDetail;
	float yDetail;
	float xCount;
	float yCount;
};