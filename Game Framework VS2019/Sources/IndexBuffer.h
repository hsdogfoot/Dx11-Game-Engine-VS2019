#pragma once
#include "Base.h"

class IndexBuffer final : public Base
{
public:
	static IndexBuffer* CreateOrNull(ID3D11Device* device, DWORD* data, UINT numElements);

private:
	IndexBuffer(UINT numElements);
	virtual ~IndexBuffer() override = default;

	bool initialize(ID3D11Device* device, DWORD* data);

	virtual void destroy() override;

public:
	ID3D11Buffer* Buffer;
	UINT BufferSize;
};