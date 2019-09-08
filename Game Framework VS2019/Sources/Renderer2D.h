#pragma once
#include "Renderer.h"

class RectBuffer;
class PixelShader;
class Texture;
class VertexShader;

class Renderer2D final : public Renderer
{
public:
	static Renderer2D* Create();
	Renderer2D* CloneOrNull(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* textureTag, const WCHAR* vsTag, const WCHAR* psTag);

	virtual uint8_t Update() override;
	virtual void Render() const override;

private:
	Renderer2D();
	explicit Renderer2D(const Renderer2D& other);
	virtual ~Renderer2D() override = default;

	bool initializeClone(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const WCHAR* textureTag, const WCHAR* vsTag, const WCHAR* psTag);

	virtual void destroy() override;

public:
	VertexShader* VShader;
	PixelShader* PShader;

private:
	RectBuffer* mRectBuffer;
	Texture* mTexture;
};