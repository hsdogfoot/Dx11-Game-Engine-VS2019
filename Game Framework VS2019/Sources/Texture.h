#pragma once
#include "Resource.h"

#include <WICTextureLoader.h>

class Texture final : public Resource
{
public:
	static Texture* CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path);

	void ReadyRender() const;

private:
	Texture(ID3D11DeviceContext* deviceContext);
	virtual ~Texture() override = default;

	bool initialize(ID3D11Device* device, const WCHAR* path);

	virtual void destroy() override;

private:
	ID3D11ShaderResourceView* mTexture;

	ID3D11DeviceContext* mDeviceContext;
};