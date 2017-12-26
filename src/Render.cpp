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

		return hr == S_OK;
}