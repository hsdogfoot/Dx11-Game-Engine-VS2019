#include "stdafx.h"
#include "Renderer2D.h"

#include "Actor.h"
#include "CameraManager.h"
#include "ResourceFactory.h"
#include "Resources.h"

Renderer2D* Renderer2D::Create()
{
	return new Renderer2D();
}

Renderer2D* Renderer2D::CloneOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* textureTag, const WCHAR* vsTag, const WCHAR* psTag)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(textureTag != nullptr);
	Assert(vsTag != nullptr);
	Assert(psTag != nullptr);

	Renderer2D* instance = new Renderer2D(*this);
	if (!instance->initializeClone(device, deviceContext, textureTag, vsTag, psTag))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

uint8_t Renderer2D::Update()
{
	return 0;
}

void Renderer2D::Render() const
{
	XMMATRIX world = XMMatrixTranspose(static_cast<Transform*>(Owner->GetComponent(L"Transform"))->World);
	XMMATRIX view = XMMatrixTranspose(CameraManager::GetInstance()->MainCamera->GetView());
	XMMATRIX projection = XMMatrixTranspose(CameraManager::GetInstance()->MainCamera->GetProjection());

	VShader->SetWVP(world, view, projection);
	VShader->ReadyRender();
	PShader->ReadyRender();

	mRectBuffer->ReadyRender();
	mTexture->ReadyRender();

	mDeviceContext->DrawIndexed(mRectBuffer->GetBufferSize(), 0, 0);
}

Renderer2D::Renderer2D()
	: Renderer()
	, VShader(nullptr)
	, PShader(nullptr)
	, mRectBuffer(nullptr)
	, mTexture(nullptr)
{
}

Renderer2D::Renderer2D(const Renderer2D& other)
	: Renderer()
	, VShader(other.VShader)
	, PShader(other.PShader)
	, mRectBuffer(other.mRectBuffer)
	, mTexture(other.mTexture)
{
}

bool Renderer2D::initializeClone(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* textureTag, const WCHAR* vsTag, const WCHAR* psTag)
{
	mDevice = device;
	mDevice->AddRef();
	mDeviceContext = deviceContext;
	mDeviceContext->AddRef();

	VShader = static_cast<VertexShader*>(ResourceFactory::GetInstance()->GetCloneOrNull(vsTag));
	if (VShader == nullptr)
	{
		ErrorLog("VertexShader clone failed.");

		return false;
	}
	VShader->AddRefCount();

	PShader = static_cast<PixelShader*>(ResourceFactory::GetInstance()->GetCloneOrNull(psTag));
	if (PShader == nullptr)
	{
		ErrorLog("PixelShader clone failed.");

		return false;
	}
	PShader->AddRefCount();

	mRectBuffer = static_cast<RectBuffer*>(ResourceFactory::GetInstance()->GetCloneOrNull(L"RectBuffer"));
	if (mRectBuffer == nullptr)
	{
		ErrorLog("RectBuffer clone failed.");

		return false;
	}
	mRectBuffer->AddRefCount();

	mTexture = static_cast<Texture*>(ResourceFactory::GetInstance()->GetCloneOrNull(textureTag));
	if (mTexture == nullptr)
	{
		ErrorLog("Texture clone failed.");

		return false;
	}
	mTexture->AddRefCount();

	return true;
}

void Renderer2D::destroy()
{
	ReleaseBase(VShader);
	ReleaseBase(PShader);

	ReleaseBase(mRectBuffer);
	ReleaseBase(mTexture);

	Renderer::destroy();
}
