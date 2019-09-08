#include "stdafx.h"
#include "VertexShader.h"

VertexShader::VertexShader(ID3D11DeviceContext* deviceContext)
	: Resource()
	, CBufferWVP(nullptr)
	, mShader(nullptr)
	, mShaderBuffer(nullptr)
	, mInputLayout(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

void VertexShader::SetWVP(XMMATRIX world, XMMATRIX view, XMMATRIX projection)
{
	CBufferWVP->Data.World = world;
	CBufferWVP->Data.View = view;
	CBufferWVP->Data.Projection = projection;

	CBufferWVP->ApplyChanges();
}

void VertexShader::destroy()
{
	ReleaseBase(CBufferWVP);

	ReleaseCOM(mShader);
	ReleaseCOM(mShaderBuffer);

	ReleaseCOM(mInputLayout);

	ReleaseCOM(mDeviceContext);
}