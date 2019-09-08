#pragma once
#include "Base.h"

class Graphics final : public Base
{
public:
	static Graphics* CreateOrNull(HWND hWND, int width, int height);

	void RenderFrame() const;

private:
	Graphics();
	virtual ~Graphics() override = default;

	bool initialize(HWND hWND, int width, int height);
	bool initializeDirectX(HWND hWND, int width, int height);

	virtual void destroy() override;

public:
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

private:
	IDXGISwapChain* mSwapChain;
	ID3D11RenderTargetView* mRenderTargetView;

	ID3D11DepthStencilView* mDepthStencilView;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilState* mDepthStencilState;

	ID3D11RasterizerState* mRasterizerState;
	ID3D11SamplerState* mSamplerState;
};