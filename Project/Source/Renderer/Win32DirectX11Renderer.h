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

#include <d3d11.h>
#include <assert.h>
#include "RendererInterface.h"

#define AssertHResult(_hr) assert(_hr >= 0)

namespace OC
{
	class Renderer final : public RendererInterface
	{
	private:
		static Renderer* s_Instance; // Private singleton used to ensure only one instance of this class exists.

		HWND m_WindowHandle; // Handle to the window.
		ID3D11Device* m_d3dDevice;
		ID3D11DeviceContext* m_d3dDeviceContext;
		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_renderTargetView;

		// Description: Releases an IUnknown object. Fails safely if the pointer points to nullptr.
		// Parameters: 
		//    typename T, ID3D11 type.
		//    IUnknown* _object, the object to release.
		inline void SafeRelease(IUnknown* _object)
		{
			if (_object)
				_object->Release();
		}

		// Description: Resize the the renderer.
		void Resize();

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

#undef assertHResult

#endif //defined(WIN32)