#pragma once
#include "Resource.h"

#include "ConstantBuffer.h"

class VertexShader abstract : public Resource
{
public:
	VertexShader(ID3D11DeviceContext* deviceContext);
	virtual ~VertexShader() override = default;

	void SetWVP(XMMATRIX world, XMMATRIX view, XMMATRIX projection);
	virtual void SetCBufferData(void* data) = 0;

	virtual void ReadyRender() = 0;

protected:
	virtual void destroy() override;

public:
	ConstantBuffer<CB_VS_WVP>* CBufferWVP;

protected:
	ID3D11VertexShader* mShader;
	ID3D10Blob* mShaderBuffer;

	ID3D11InputLayout* mInputLayout;
	
	ID3D11DeviceContext* mDeviceContext;
};