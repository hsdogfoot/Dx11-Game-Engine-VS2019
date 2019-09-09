#pragma once
#include "PixelShader.h"

#include "stdafx.h"

template <typename Type>
class TPixelShader final : public PixelShader
{
public:
	static TPixelShader<Type>* CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::wstring path);

	virtual void SetCBufferData(void* data) override;

	virtual void ReadyRender() override;

private:
	TPixelShader(ID3D11DeviceContext* deviceContext);
	virtual ~TPixelShader() override = default;

	bool initialize(ID3D11Device* device, std::wstring path);

	virtual void destroy() override;

public:
	ConstantBuffer<Type>* CBuffer;
};

template<typename Type>
TPixelShader<Type>* TPixelShader<Type>::CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::wstring path)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(!path.empty());

	TPixelShader<Type>* instance = new TPixelShader<Type>(deviceContext);
	if (!instance->initialize(device, path))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

template<typename Type>
void TPixelShader<Type>::SetCBufferData(void* data)
{
	Assert(data != nullptr);

	CBuffer->SetData(data);
}

template<typename Type>
void TPixelShader<Type>::ReadyRender()
{
	mDeviceContext->PSSetShader(mShader, nullptr, 0);

	CBuffer->ApplyChanges();

	mDeviceContext->PSSetConstantBuffers(1, 1, &CBuffer->Buffer);
}

template<typename Type>
TPixelShader<Type>::TPixelShader(ID3D11DeviceContext* deviceContext)
	: PixelShader(deviceContext)
	, CBuffer(nullptr)
{
}

template<typename Type>
bool TPixelShader<Type>::initialize(ID3D11Device* device, std::wstring path)
{
	HRESULT hResult = D3DReadFileToBlob(path.c_str(), &mShaderBuffer);
	if (FAILED(hResult))
	{
		HRAndPathLog(hResult, path);

		return false;
	}

	hResult = device->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	CBuffer = ConstantBuffer<Type>::CreateOrNull(device, mDeviceContext);
	if (CBuffer == nullptr)
	{
		ErrorLog("Constant buffer create failed.");

		return false;
	}

	return true;
}

template<typename Type>
void TPixelShader<Type>::destroy()
{
	ReleaseBase(CBuffer);

	PixelShader::destroy();
}
