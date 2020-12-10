/*
-------------------------------------------------------------------------------------------------------
	File: Win32DirectX11Renderer.cpp	
	Author: Ozzie Mercado
	Created: December 9, 2020
	Modified: December 10, 2020
	Note: See header for more documentation.
-------------------------------------------------------------------------------------------------------
*/

#include <assert.h>
#include "Win32DirectX11Renderer.h"

namespace OC
{
	// private

	Renderer* Renderer::s_Instance = nullptr;

	void Renderer::Resize() // TODO: A way to call this when the window resizes. Event System or intercepting window messages would help.
	{
		// Resize swap chain.
		AssertHResult( m_swapChain->ResizeBuffers(2, 0, 0, DXGI_FORMAT_B8G8R8A8_UNORM, 0) );
	}

	// public

	Renderer::Renderer(const Window& _window) :
		RendererInterface(_window),
		m_WindowHandle(nullptr),
		m_d3dDevice(nullptr),
		m_d3dDeviceContext(nullptr),
		m_swapChain(nullptr),
		m_renderTargetView(nullptr)
	{
		assert(!s_Instance); // Error: There can only be one instance of Renderer.

		s_Instance = this;
		m_WindowHandle = static_cast<HWND>(_window.GetHandle());

		// Create the device and swap chain.

		UINT layerFlags = 0;
#if !defined(NDEBUG)
		layerFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		RECT clientRect;
		GetClientRect(m_WindowHandle, &clientRect);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Width = static_cast<unsigned int>(clientRect.right - clientRect.left);
		swapChainDesc.BufferDesc.Height = static_cast<unsigned int>(clientRect.bottom - clientRect.top);
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.OutputWindow = m_WindowHandle;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Use DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL when using 2 buffers.
		swapChainDesc.Flags = 0;

		D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 }; // One for now, until the rendering system is more feature complete.
		D3D_FEATURE_LEVEL featureLevelSupported;

		AssertHResult(
			D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				layerFlags,
				featureLevels,
				1,
				D3D11_SDK_VERSION,
				&swapChainDesc,
				&m_swapChain,
				&m_d3dDevice,
				&featureLevelSupported,
				&m_d3dDeviceContext
			)
		);

		// Create the render target view.

		ID3D11Texture2D* backBuffer;
		AssertHResult(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		AssertHResult(m_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView));
		backBuffer->Release();


		// Specify the viewport.

		D3D11_TEXTURE2D_DESC backBufferDesc;
		ZeroMemory(&backBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
		backBuffer->GetDesc(&backBufferDesc);

		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(backBufferDesc.Width);
		viewport.Height = static_cast<float>(backBufferDesc.Height);
		viewport.MinDepth = D3D11_MIN_DEPTH;
		viewport.MaxDepth = D3D11_MAX_DEPTH;

		m_d3dDeviceContext->RSSetViewports(1, &viewport);
	}

	Renderer::~Renderer()
	{
		s_Instance = nullptr;

		SafeRelease(m_d3dDevice);
		SafeRelease(m_d3dDeviceContext);
		SafeRelease(m_swapChain);
		SafeRelease(m_renderTargetView);
	}

	void Renderer::Present()
	{
		// Specify the render target.
		m_d3dDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr); // No depth stencil for now.

		// Clear the render target.
		constexpr float clearColor[4] = { 0.2f, 0.4f, 0.8f, 1.0f };
		m_d3dDeviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);

		// Present the rendered image to the window.
		AssertHResult( m_swapChain->Present(0, 0) ); // m_swapChain->Present(1, 0) for 2 buffers
	}
}
