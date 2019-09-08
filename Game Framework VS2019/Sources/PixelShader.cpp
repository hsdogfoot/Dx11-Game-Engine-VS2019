#include "stdafx.h"
#include "PixelShader.h"

PixelShader::PixelShader(ID3D11DeviceContext* deviceContext)
	: Resource()
	, mShader(nullptr)
	, mShaderBuffer(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

void PixelShader::destroy()
{
	ReleaseCOM(mShader);
	ReleaseCOM(mShaderBuffer);

	ReleaseCOM(mDeviceContext);
}