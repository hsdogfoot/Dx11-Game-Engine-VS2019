#pragma once
#include "Resource.h"

#include <WICTextureLoader.h>
#include "ConstantBuffer.h"

class Texture final : public Resource
{
public:
	struct TextureDesc
	{
		bool IsAnimation;
		int TotalCount;
		int ColumnCount;
		int RowCount;
	};

public:
	static Texture* CreateOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* path, TextureDesc* textureDesc);

	void ReadyRender();

private:
	Texture(ID3D11DeviceContext* deviceContext, TextureDesc* textureDesc);
	virtual ~Texture() override = default;

	bool initialize(ID3D11Device* device, const WCHAR* path);

	virtual void destroy() override;

private:
	ID3D11ShaderResourceView* mTexture;

	TextureDesc mTextureDesc;
	ConstantBuffer<CB_PS_OFFSET>* mCBuffer;

	int mAnimationCount;
	int mTimeCount;

	ID3D11DeviceContext* mDeviceContext;
};