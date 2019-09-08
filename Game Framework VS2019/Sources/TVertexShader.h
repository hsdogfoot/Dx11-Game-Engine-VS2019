#pragma once
#include "VertexShader.h"

#include "stdafx.h"

template <typename Type>
class TVertexShader final : public VertexShader
{
public:
	static TVertexShader<Type>* CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::wstring path);

	virtual void SetCBufferData(void* data) override;

	virtual void ReadyRender() override;

private:
	TVertexShader(ID3D11DeviceContext* deviceContext);
	virtual ~TVertexShader() override = default;

	bool initialize(ID3D11Device* device, std::wstring path);

	virtual void destroy() override;

public:
	ConstantBuffer<Type>* CBuffer;
};

template<typename Type>
TVertexShader<Type>* TVertexShader<Type>::CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, std::wstring path)
{
	Assert(device != nullptr);
	Assert(deviceContext != nullptr);
	Assert(!path.empty());

	TVertexShader<Type>* instance = new TVertexShader<Type>(deviceContext);
	if (!instance->initialize(device, path))
	{
		instance->Release();

		return nullptr;
	}

	return instance;
}

template<typename Type>
void TVertexShader<Type>::SetCBufferData(void* data)
{
	Assert(data != nullptr);

	CBuffer->SetData(data);
}

template<typename Type>
void TVertexShader<Type>::ReadyRender()
{
	mDeviceContext->IASetInputLayout(mInputLayout);
	mDeviceContext->VSSetShader(mShader, nullptr, 0);

	CBuffer->ApplyChanges();

	mDeviceContext->VSSetConstantBuffers(0, 1, &CBufferWVP->Buffer);
	mDeviceContext->VSSetConstantBuffers(1, 1, &CBuffer->Buffer);
}

template<typename Type>
TVertexShader<Type>::TVertexShader(ID3D11DeviceContext* deviceContext)
	: VertexShader(deviceContext)
	, CBuffer(nullptr)
{
}

template<typename Type>
bool TVertexShader<Type>::initialize(ID3D11Device* device, std::wstring path)
{
	HRESULT hResult = D3DReadFileToBlob(path.c_str(), &mShaderBuffer);
	if (FAILED(hResult))
	{
		HRAndPathLog(hResult, path);

		return false;
	}

	hResult = device->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mShader);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hResult = device->CreateInputLayout(layout, ARRAYSIZE(layout), mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout);
	if (FAILED(hResult))
	{
		HRLog(hResult);

		return false;
	}

	CBufferWVP = ConstantBuffer<CB_VS_WVP>::CreateOrNull(device, mDeviceContext);
	if (CBufferWVP == nullptr)
	{
		ErrorLog("Constant buffer create failed.");

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
void TVertexShader<Type>::destroy()
{
	ReleaseBase(CBuffer);

	VertexShader::destroy();
}
