#include "stdafx.h"
#include "Texture.h"

Texture* Texture::CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, TextureDesc* textureDesc)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);

	Texture* instance = new Texture(deviceContext, textureDesc);
	if (!instance->initialize(device, path))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

void Texture::ReadyRender()
{
	mDeviceContext->PSSetShaderResources(0, 1, &mTexture);

	if (mTextureDesc.IsAnimation)
	{
		++mTimeCount;

		if (mTimeCount > 4)
		{
			mTimeCount = 0;
			++mAnimationCount;

			if (mAnimationCount >= mTextureDesc.TotalCount)
			{
				mAnimationCount = 0;
			}
		}

		mCBuffer->Data.xDetail = 1.0f / mTextureDesc.ColumnCount;
		mCBuffer->Data.yDetail = 1.0f / mTextureDesc.RowCount;
		mCBuffer->Data.xCount = static_cast<float>(mAnimationCount % mTextureDesc.ColumnCount);
		mCBuffer->Data.yCount = static_cast<float>(mAnimationCount / mTextureDesc.ColumnCount);
	}

	mCBuffer->ApplyChanges();

	mDeviceContext->PSSetConstantBuffers(0, 1, &mCBuffer->Buffer);
}

Texture::Texture(ID3D11DeviceContext* deviceContext, TextureDesc* textureDesc)
	: Resource()
	, mTexture(nullptr)
	, mTextureDesc(*textureDesc)
	, mCBuffer(nullptr)
	, mAnimationCount(0)
	, mTimeCount(0)
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

	mCBuffer = ConstantBuffer<CB_PS_OFFSET>::CreateOrNull(device, mDeviceContext);
	if (mCBuffer == nullptr)
	{
		return false;
	}

	return true;
}

void Texture::destroy()
{
	ReleaseCOM(mTexture);

	ReleaseBase(mCBuffer);

	ReleaseCOM(mDeviceContext);
}
