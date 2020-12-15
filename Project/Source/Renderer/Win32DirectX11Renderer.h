/*
-------------------------------------------------------------------------------------------------------
	File: Win32DirectX11Renderer.h
	Author: Ozzie Mercado
	Created: December 9, 2020
	Modified: December 10, 2020
	Description: The Win32 implementation of the renderer interface. Creates a renderer, sets it up
		to output to a given window, and presents rendered images to the screen.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

#if defined(WIN32)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include "Shaders/ConstantBufferStructs.hlsli"
#include "RendererInterface.h"

namespace OC
{
	class Renderer final : public RendererInterface
	{
	private:
		static Renderer* s_Instance; // Private singleton used to ensure only one instance of this class exists.

		HWND m_WindowHandle; // Handle to the window.
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_SwapChain;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11Buffer* m_ConstantBuffer;
		ID3D11VertexShader* m_VertexShader;
		ID3D11PixelShader* m_PixelShader;
		ID3D11InputLayout* m_InputLayout;
		ID3D11RasterizerState* m_RasterState;

		MVP m_MVP;
		float m_Aspect;

		// Description: Determines the supported shader profile and creates a string representation of it.
		// Parameters: 
		//    char* _outVertexProfileStr, the string representation of the shader profile.
		// Returns: true, if DirectX feature level is supported and _outProfileStr was set.
		bool GetVertexShaderProfileString(std::string& _outProfileStr) const;
		bool GetPixelShaderProfileString(std::string& _outProfileStr) const;

		// Description: Creates a shader using a given shader blob.
		// Parameters: 
		//    ID3DBlob* _shaderBlob, blob to use when creating the shader.
		// Returns: The created shader or nullptr if unsuccessful.
		ID3D11VertexShader* CreateVertexShader(ID3DBlob* _shaderBlob) const;
		ID3D11PixelShader* CreatePixelShader(ID3DBlob* _shaderBlob) const;

		// Description: Compiles an hlsl vertex shader, creates input layout for it, and returns the shader.
		//    Also, outputs to the console if there is an error during compilation or with file IO.
		// Parameters: 
		//    const wchar_t* _fileName, name and path to the hlsl file.
		//    D3D11_INPUT_ELEMENT_DESC* _layoutDescs, descriptions for the input-assembler.
		//    unsigned int _numLayoutDescs, number of descriptions for the input-assembler.
		//    ID3D11InputLayout** _outInputLayout, the returned input layout if it can be created.
		// Returns: The created shader or nullptr if unsuccessful.
		ID3D11VertexShader* CompileAndLoadVertexShader(const wchar_t* _fileName,
													   D3D11_INPUT_ELEMENT_DESC* _layoutDescs,
													   unsigned int _numLayoutDescs,
													   ID3D11InputLayout** _outInputLayout);

		// Description: Compiles an hlsl pixel shader and returns it. Also, outputs to the console if there is
		//    an error during compilation or with file IO.
		// Parameters: 
		//    const wchar_t* _fileName, name and path to the hlsl file.
		// Returns: The created shader or nullptr if unsuccessful.
		ID3D11PixelShader* CompileAndLoadPixelShader(const wchar_t* _fileName);

		// Description: Releases an IUnknown object, which ID3D11 objects inherit from. Fails safely if the
		//    pointer points to nullptr.
		// Parameters: 
		//    IUnknown* _object, the object to release.
		inline void SafeRelease(IUnknown* _object)
		{
			if (_object)
				_object->Release();
		}
		
		// Description: Resize the the renderer.
		void Resize();

		// Description: Draws a triangle to the render target.
		void DrawTestTriangle();

	public:
		// Description: Constructs the renderer system and sets it up to output to the window.
		// Parameters: 
		//    const Window& _window, the window to render to.
		Renderer(const Window& _window);

		// Description: Remove the renderer from the window and clean up this instance.
		~Renderer();

		// Description: Renders to the window.
		void Present();
	};
}

#endif //defined(WIN32)