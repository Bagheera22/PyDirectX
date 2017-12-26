#include <Render.h>

bool Context::Create(HWND hWnd, const int width, const int height)
{
		ZeroMemory(&swapChainDesc_, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc_.BufferCount = 1;
		swapChainDesc_.BufferDesc.Width = width;
		swapChainDesc_.BufferDesc.Height = height;
		swapChainDesc_.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc_.SampleDesc.Count = 4;
		swapChainDesc_.SampleDesc.Quality = 0;
		swapChainDesc_.Windowed = true;
		swapChainDesc_.OutputWindow = hWnd;
		swapChainDesc_.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc_.BufferDesc.RefreshRate.Denominator = 1;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
			NULL, NULL, D3D11_SDK_VERSION,
			&swapChainDesc_, &swapChain_, &device_,
			NULL, &context_);

		if (FAILED(hr))
		{
			return false;
		}

		hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D),
			(LPVOID*)&backBuffer_);
		
		if (FAILED(hr))
		{
			return false;
		}

		hr = device_->CreateRenderTargetView(backBuffer_, NULL, &rtView_);
		
		if (FAILED(hr))
		{
			return false;
		}
		ZeroMemory(&viewport_, sizeof(D3D11_VIEWPORT));
		viewport_.MinDepth = 0.0f;
		viewport_.MaxDepth = 1.0f;
		viewport_.TopLeftX = 0;
		viewport_.TopLeftY = 0;
		viewport_.Width = static_cast<float>(width);
		viewport_.Height = static_cast<float>(height);
		return hr == S_OK;
}

void Context::Render()
{
	float clearColor[] = { 0.3f, 0.9f, 0.3f, 1.0f };

	context_->OMSetRenderTargets(1, &rtView_, NULL);
	context_->RSSetViewports(1, &viewport_);
	context_->ClearRenderTargetView(rtView_, clearColor);

	//context_->IASetInputLayout(layout_);

	swapChain_->Present(0, 0);
}
