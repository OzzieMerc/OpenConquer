/*
-------------------------------------------------------------------------------------------------------
	File: Win32Window.h
	Author: Ozzie Mercado
	Created: December 6, 2020
	Modified: December 7, 2020
	Description: The Win32 implementation of the window interface. Opens, closes, and updates a window.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

#if defined(WIN32)

#include <windows.h>
#include "WindowInterface.h"

namespace OC
{
	class Window final : public WindowInterface
	{
	private:
		HWND m_WindowHandle; // Handle to the window.
		HDC m_DeviceContextHandle; // Handle to the window's device context.
		HINSTANCE m_InstanceHandle; // Handle to the window's instance.

		// Description: Handles window messages.
		// Parameters: 
		//    HWND _hWnd, handle to the window.
		//    UINT _message, the message code.
		//    WPARAM _wParam, additional data pertaining to the message.
		//    LPARAM _lParam, additional data pertaining to the message.
		// Returns: The repsonse to the message received.
		static LRESULT CALLBACK WindowPocedure(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

		// Description: Displays an error message on the screen.
		// Parameters: 
		//    const wchar_t* _message, the message to display.
		void DisplayError(const wchar_t* _message);

	public:
		// Description: Constructs the window and opens it.
		// Parameters: 
		//    const wchar_t* _name, the text to place in the caption area (titlebar) of the window.
		//    int _x, the x position of the window.
		//    int _y, the y position of the window.
		//    unsigned int _width, the horizontal width of the drawable area of the window.
		//    unsigned int _height, the vertical height of the drawable area of the window.
		Window(const wchar_t* _name, int _x, int _y, unsigned int _width, unsigned int _height);

		// Description: Closes the window if it's open, and cleans up references.
		~Window();

		// Description: Registers and opens a window.
		// Returns: true, if the window opened.
		bool Open();

		// Description: Closes the window and unregisters.
		void Close();

		// Description: Runs the window message loop.
		// Returns: false, if the window is closed.
		bool Update();

		// Description: Returns a handle to the window.
		// Returns: Pointer to the window handle.
		void* GetHandle() const;
	};
}

#endif //defined(WIN32)