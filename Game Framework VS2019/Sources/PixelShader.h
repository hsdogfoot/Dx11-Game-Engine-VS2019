#pragma once
#include "Resource.h"

#include "ConstantBuffer.h"

class PixelShader abstract : public Resource
{
public:
	PixelShader(ID3D11DeviceContext* deviceContext);
	virtual ~PixelShader() override = default;

	virtual void SetCBufferData(void* data) = 0;

	virtual void ReadyRender() = 0;

protected:
	virtual void destroy() override;

protected:
	ID3D11PixelShader* mShader;
	ID3D10Blob* mShaderBuffer;

	ID3D11DeviceContext* mDeviceContext;
};