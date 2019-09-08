#pragma once
#include "Resource.h"

template <typename T>
class VertexBuffer;
class IndexBuffer;

struct Vertex;

class RectBuffer final : public Resource
{
public:
	static RectBuffer* CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	void ReadyRender() const;

	UINT GetBufferSize() const;

private:
	RectBuffer(ID3D11DeviceContext* deviceContext);
	virtual ~RectBuffer() override = default;

	bool initialize(ID3D11Device* device);
	bool createVertexBuffer(ID3D11Device* device);
	bool createIndexBuffer(ID3D11Device* device);

	virtual void destroy() override;

private:
	VertexBuffer<Vertex>* mVBuffer;
	IndexBuffer* mIBuffer;

	ID3D11DeviceContext* mDeviceContext;
};