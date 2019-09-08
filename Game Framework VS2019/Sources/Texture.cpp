#include "stdafx.h"
#include "Texture.h"

Texture* Texture::CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	Texture* instance = new Texture(deviceContext);
	if (!instance->initialize(device, path))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

void Texture::ReadyRender() const
{
	mDeviceContext->PSSetShaderResources(0, 1, &mTexture);
}

Texture::Texture(ID3D11DeviceContext* deviceContext)
	: Resource()
	, mTexture(nullptr)
	, mDeviceContext(deviceContext)
{
	mDeviceContext->AddRef();
}

bool Texture::initialize(ID3D11Device* device, const WCHAR* path)
{
	HRESULT hResult = DirectX::CreateWICTextureFromFile(device, path, nullptr, &mTexture);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	return true;
}

void Texture::destroy()
{
	ReleaseCOM(mTexture);

	ReleaseCOM(mDeviceContext);
}
