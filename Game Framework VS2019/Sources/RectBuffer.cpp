#include "stdafx.h"
#include "RectBuffer.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"

RectBuffer* RectBuffer::CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	RectBuffer* instance = new RectBuffer(deviceContext);
	if (!instance->initialize(device))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

void RectBuffer::ReadyRender() const
{
	mDeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	mDeviceContext->IASetVertexBuffers(0, 1, &mVBuffer->Buffer, mVBuffer->Stride.get(), &offset);
	mDeviceContext->IASetIndexBuffer(mIBuffer->Buffer, DXGI_FORMAT_R32_UINT, 0);
}

UINT RectBuffer::GetBufferSize() const
{
	return mIBuffer->BufferSize;
}

RectBuffer::RectBuffer(ID3D11DeviceContext* deviceContext)
	: Resource()
	, mVBuffer(nullptr)
	, mIBuffer(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

bool RectBuffer::initialize(ID3D11Device* device)
{
	if (!createVertexBuffer(device))
	{
		ErrorLog("Create vertex buffer failed.");

		return false;
	}

	if (!createIndexBuffer(device))
	{
		ErrorLog("Create index buffer failed.");

		return false;
	}

	return true;
}

bool RectBuffer::createVertexBuffer(ID3D11Device* device)
{
	Vertex vertices[] =
	{
		Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
		Vertex(-0.5f, +0.5f, 0.0f, 0.0f, 0.0f),
		Vertex(+0.5f, +0.5f, 0.0f, 1.0f, 0.0f),
		Vertex(+0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
	};

	mVBuffer = VertexBuffer<Vertex>::CreateOrNull(device, vertices, ARRAYSIZE(vertices));
	if (mVBuffer == nullptr)
	{
		ErrorLog("VertexBuffer create failed.");

		return false;
	}

	return true;
}

bool RectBuffer::createIndexBuffer(ID3D11Device* device)
{
	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	mIBuffer = IndexBuffer::CreateOrNull(device, indices, ARRAYSIZE(indices));
	if (mIBuffer == nullptr)
	{
		ErrorLog("IndexBuffer create failed.");

		return false;
	}

	return true;
}

void RectBuffer::destroy()
{
	ReleaseBase(mVBuffer);
	ReleaseBase(mIBuffer);

	ReleaseCOM(mDeviceContext)
}
