#pragma once

#include <Windows.h>
#include <d3d11.h>

class Context
{
public:
	bool Create(HWND hWnd, const int width, const int height);
	void Render();

private:

	ID3D11Device* device_;
	ID3D11DeviceContext* context_;
	IDXGISwapChain* swapChain_;
	ID3D11RenderTargetView* rtView_;
	DXGI_SWAP_CHAIN_DESC swapChainDesc_;
	ID3D11Texture2D* backBuffer_;
	D3D11_VIEWPORT viewport_;
	IDXGIAdapter* adapter_;
	ID3D11VertexShader* vshader_;
	ID3D11PixelShader* pshader_;
	ID3D11InputLayout* layout_;
	ID3D11Buffer* vbuffer_;
	ID3D11Buffer* ibuffer_;
	ID3D11Buffer* cbPerObject_;
	ID3D11Buffer* cb2_;

};
