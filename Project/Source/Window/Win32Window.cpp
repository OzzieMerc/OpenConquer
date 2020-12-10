/*
-------------------------------------------------------------------------------------------------------
	File: Win32Window.cpp	
	Author: Ozzie Mercado
	Created: December 6, 2020
	Modified: December 7, 2020
	Note: See header for more documentation.
-------------------------------------------------------------------------------------------------------
*/

#include <assert.h>
#include "Win32Window.h"

namespace OC
{
	// private

	LRESULT CALLBACK Window::WindowPocedure(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
	{
		switch (_message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProcW(_hWnd, _message, _wParam, _lParam);
	}

	void Window::DisplayError(const wchar_t* _message)
	{
		assert(_message); // Error: _message is nullptr.

		MessageBoxW(
			NULL,
			_message,
			L"Window Error",
			NULL
		);
	}

	// public

	Window::Window(const wchar_t* _name, int _x, int _y, unsigned int _width, unsigned int _height) :
		WindowInterface(_name, _x, _y, _width, _height),
		m_WindowHandle(nullptr),
		m_DeviceContextHandle(nullptr),
		m_InstanceHandle(nullptr)
	{
		Open();
	}

	Window::~Window()
	{
		Close();
	}
	
	bool Window::Open()
	{
		assert(!m_WindowHandle); // Error: A window already exists.

		// Register the window class.
		WNDCLASSEXW  wndClass;
		ZeroMemory(&wndClass, sizeof(WNDCLASSEXW));
		wndClass.cbSize = sizeof(WNDCLASSEXW);
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WindowPocedure;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = GetModuleHandleW(0);
		wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor(0, IDC_ARROW);
		wndClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOWFRAME);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = m_Name;
		wndClass.hIconSm = LoadIcon(wndClass.hInstance, IDI_APPLICATION);

		if (!RegisterClassExW(&wndClass))
		{
			DisplayError(L"RegisterClassExW failed");
			return false;
		}

		// Define the primary characteristics of the window.
		RECT viewDimensions = { 0, 0, static_cast<LONG>(m_Width), static_cast<LONG>(m_Height) };
		DWORD style = WS_POPUPWINDOW | WS_CAPTION;

		// Adjust the view dimensions to include the window frame.
		AdjustWindowRect(&viewDimensions, style, false);

		// Create the window.
		m_WindowHandle = CreateWindowW(
			m_Name, m_Name, style,
			m_X, m_Y,
			viewDimensions.right - viewDimensions.left,
			viewDimensions.bottom - viewDimensions.top,
			NULL, NULL, wndClass.hInstance, 0
		);

		if (!m_WindowHandle)
		{
			DisplayError(L"CreateWindowW failed");
			return false;
		}

		ShowWindow(m_WindowHandle, SW_SHOW);
		m_DeviceContextHandle = GetDC(m_WindowHandle);
		m_InstanceHandle = wndClass.hInstance;

		return true;
	}

	void Window::Close()
	{
		if (m_WindowHandle)
		{
			if (IsWindow(m_WindowHandle))
				DestroyWindow(m_WindowHandle);

			m_WindowHandle = nullptr;
		}

		if (m_InstanceHandle)
		{
			UnregisterClassW(m_Name, m_InstanceHandle);
			m_InstanceHandle = nullptr;
		}

		m_DeviceContextHandle = nullptr;
	}

	bool Window::Update()
	{
		assert(m_WindowHandle); // Error: Window does not exist.

		MSG message;
		ZeroMemory(&message, sizeof(message));

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessageW(&message);

			if (message.message == WM_QUIT)
			{
				Close();
				return false;
			}
		}


		return true;
	}

	void* Window::GetHandle() const
	{
		assert(m_WindowHandle); // Error: Window does not exist.

		return reinterpret_cast<void*>(m_WindowHandle);
	}
}
