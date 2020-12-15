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
#include <stdlib.h>

#define AssertHResult(_hr) assert(SUCCEEDED(_hr))
#define Static_Assert16ByteAlignment(_STRUCT) static_assert((sizeof(_STRUCT) % 16) == 0, #_STRUCT##" size must be 16-byte aligned")

// Assert that the constant buffer remains 16-byte aligned.
Static_Assert16ByteAlignment(OC::MVP);

namespace OC
{
	// private

	Renderer* Renderer::s_Instance = nullptr;

	bool Renderer::GetVertexShaderProfileString(std::string& _outProfileStr) const
	{
		assert(m_Device);

		switch (m_Device->GetFeatureLevel())
		{
		case D3D_FEATURE_LEVEL_11_0:
		{
			_outProfileStr = "vs_5_0";
			return true;
		}
		}

		return false;
	}

	bool Renderer::GetPixelShaderProfileString(std::string& _outProfileStr) const
	{
		assert(m_Device);

		switch (m_Device->GetFeatureLevel())
		{
		case D3D_FEATURE_LEVEL_11_0:
		{
			_outProfileStr = "ps_5_0";
			return true;
		}
		}

		return false;
	}

	ID3D11VertexShader* Renderer::CreateVertexShader(ID3DBlob* _shaderBlob) const
	{
		assert(m_Device);
		assert(_shaderBlob);

		ID3D11VertexShader* pVertexShader = nullptr;
		AssertHResult(
			m_Device->CreateVertexShader(
				_shaderBlob->GetBufferPointer(), 
				_shaderBlob->GetBufferSize(), 
				nullptr, 
				&pVertexShader
			)
		);

		return pVertexShader;
	}

	ID3D11PixelShader* Renderer::CreatePixelShader(ID3DBlob* _shaderBlob) const
	{
		assert(m_Device);
		assert(_shaderBlob);

		ID3D11PixelShader* pPixelShader = nullptr;
		AssertHResult(
			m_Device->CreatePixelShader(
				_shaderBlob->GetBufferPointer(),
				_shaderBlob->GetBufferSize(),
				nullptr,
				&pPixelShader
			)
		);

		return pPixelShader;
	}

	ID3D11VertexShader* Renderer::CompileAndLoadVertexShader(const wchar_t* _fileName, D3D11_INPUT_ELEMENT_DESC* _layoutDescs, unsigned int _numLayoutDescs, ID3D11InputLayout** _outInputLayout)
	{
		assert(_fileName);
		assert(_layoutDescs);
		assert(_numLayoutDescs > 0);

		std::string vertexShaderProfile;
		assert(GetVertexShaderProfileString(vertexShaderProfile));

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if !defined(NDEBUG)
		flags |= D3DCOMPILE_DEBUG;
#endif

		// Binary Large Object (BLOB)
		ID3DBlob* shaderCode = nullptr, *errors = nullptr;

		HRESULT hr = D3DCompileFromFile(
			_fileName, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", vertexShaderProfile.c_str(),
			flags, NULL, &shaderCode, &errors
		);

		ID3D11VertexShader* shader = nullptr;

		if (SUCCEEDED(hr))
		{
			shader = CreateVertexShader(shaderCode);

			AssertHResult(
				m_Device->CreateInputLayout(
					_layoutDescs, _numLayoutDescs,
					shaderCode->GetBufferPointer(),
					shaderCode->GetBufferSize(),
					_outInputLayout
				)
			);
		}
		else if (errors)
			printf("HLSL Compile Error: %s\n", reinterpret_cast<char*>(errors->GetBufferPointer()));
		else
		{
			switch (GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
				wprintf(L"Error: ERROR_FILE_NOT_FOUND - %s\n", _fileName);
				break;
			case ERROR_PATH_NOT_FOUND:
				wprintf(L"Error: ERROR_PATH_NOT_FOUND - %s\n", _fileName);
				break;
			default:
				printf("Error: %d\n", static_cast<int>(GetLastError()));
				break;
			}
		}
		
		SafeRelease(shaderCode);
		SafeRelease(errors);

		return shader;
	}

	ID3D11PixelShader* Renderer::CompileAndLoadPixelShader(const wchar_t* _fileName)
	{
		assert(_fileName);

		std::string pixelShaderProfile;
		assert(GetPixelShaderProfileString(pixelShaderProfile));

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if !defined(NDEBUG)
		flags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* shaderCode = nullptr, * errors = nullptr;

		HRESULT hr = D3DCompileFromFile(
			_fileName, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main", pixelShaderProfile.c_str(),
			flags, NULL, &shaderCode, &errors
		);

		ID3D11PixelShader* shader = nullptr;

		if (SUCCEEDED(hr))
			shader = CreatePixelShader(shaderCode);
		else if (errors)
			printf("HLSL Compile Error: %s\n", reinterpret_cast<char*>(errors->GetBufferPointer()));
		else
		{
			switch (GetLastError())
			{
			case ERROR_FILE_NOT_FOUND:
				wprintf(L"Error: ERROR_FILE_NOT_FOUND - %s\n", _fileName);
				break;
			case ERROR_PATH_NOT_FOUND:
				wprintf(L"Error: ERROR_PATH_NOT_FOUND - %s\n", _fileName);
				break;
			default:
				printf("Error: %d\n", static_cast<int>(GetLastError()));
				break;
			}
		}

		SafeRelease(shaderCode);
		SafeRelease(errors);

		return shader;
	}

	void Renderer::Resize() // TODO: A way to call this when the window resizes. Event System or intercepting window messages would help.
	{
		RECT clientRect;
		GetClientRect(m_WindowHandle, &clientRect);

		unsigned int width = static_cast<unsigned int>(clientRect.right - clientRect.left);
		unsigned int height = static_cast<unsigned int>(clientRect.bottom - clientRect.top);

		m_Aspect = width / static_cast<float>(height);

		// Resize swap chain.
		AssertHResult( m_SwapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_B8G8R8A8_UNORM, 0) );
	}

	void Renderer::DrawTestTriangle()
	{
		m_DeviceContext->IASetInputLayout(m_InputLayout);
		m_DeviceContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
		m_DeviceContext->VSSetShader(m_VertexShader, nullptr, 0);
		m_DeviceContext->PSSetShader(m_PixelShader, nullptr, 0);
		m_DeviceContext->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_MVP, 0, 0);
		m_DeviceContext->RSSetState(m_RasterState);
		m_DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_DeviceContext->Draw(3, 0);
	}

	// public

	Renderer::Renderer(const Window& _window) :
		RendererInterface(_window),
		m_WindowHandle(nullptr),
		m_Device(nullptr),
		m_DeviceContext(nullptr),
		m_SwapChain(nullptr),
		m_RenderTargetView(nullptr)
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

		unsigned int width = static_cast<unsigned int>(clientRect.right - clientRect.left);
		unsigned int height = static_cast<unsigned int>(clientRect.bottom - clientRect.top);

		m_Aspect = width / static_cast<float>(height);

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
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
				&m_SwapChain,
				&m_Device,
				&featureLevelSupported,
				&m_DeviceContext
			)
		);

		// Create the render target view.

		ID3D11Texture2D* backBuffer;
		AssertHResult(m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		AssertHResult(m_Device->CreateRenderTargetView(backBuffer, nullptr, &m_RenderTargetView));
		backBuffer->Release();
		
		// Specify the viewport.

		D3D11_TEXTURE2D_DESC backBufferDesc;
		ZeroMemory(&backBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));
		backBuffer->GetDesc(&backBufferDesc);

		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(backBufferDesc.Width);
		viewport.Height = static_cast<float>(backBufferDesc.Height);
		viewport.MinDepth = D3D11_MIN_DEPTH;
		viewport.MaxDepth = D3D11_MAX_DEPTH;

		m_DeviceContext->RSSetViewports(1, &viewport);

		// Create the rasterizer state.
		D3D11_RASTERIZER_DESC rasterDesc;
		ZeroMemory(&rasterDesc, sizeof(rasterDesc));
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.AntialiasedLineEnable = true;

		AssertHResult(m_Device->CreateRasterizerState(&rasterDesc, &m_RasterState));

		// Create constant buffer.
		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.ByteWidth = sizeof(MVP);
		constantBufferDesc.CPUAccessFlags = 0;
		constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		AssertHResult(m_Device->CreateBuffer(&constantBufferDesc, nullptr, &m_ConstantBuffer));

		// Create the input layout for the vertex shader.
		D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		// Load the shaders
		m_VertexShader = CompileAndLoadVertexShader(L"../../Project/Source/Renderer/Shaders/vsTriangle.hlsl", vertexLayoutDesc, 2, &m_InputLayout);
		m_PixelShader = CompileAndLoadPixelShader(L"../../Project/Source/Renderer/Shaders/psTriangle.hlsl");
	}

	Renderer::~Renderer()
	{
		s_Instance = nullptr;

		SafeRelease(m_Device);
		SafeRelease(m_DeviceContext);
		SafeRelease(m_SwapChain);
		SafeRelease(m_RenderTargetView);
		SafeRelease(m_ConstantBuffer);
		SafeRelease(m_VertexShader);
		SafeRelease(m_PixelShader);
		SafeRelease(m_InputLayout);
		SafeRelease(m_RasterState);
	}

	void Renderer::Present()
	{
		// Specify the render target.
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, nullptr); // No depth stencil for now.

		// Clear the render target.
		constexpr float clearColor[4] = { 0.2f, 0.4f, 0.8f, 1.0f };
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, clearColor);

		DrawTestTriangle();

		// Present the rendered image to the window.
		AssertHResult( m_SwapChain->Present(0, 0) ); // m_swapChain->Present(1, 0) for 2 buffers
	}
}

#undef AssertHResult
#undef Static_Assert16ByteAlignment
