/*
-------------------------------------------------------------------------------------------------------
	File: Win32Input.h
	Author: Ozzie Mercado
	Created: December 7, 2020
	Modified: December 8, 2020
	Description: The Win32 implementation of the input interface. Keeps track of key and mouse states,
		while providing functions for retrieving information about them.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

#if defined(WIN32)

#include <assert.h>
#include <bitset>
#include "Win32Keys.h"
#include "InputInterface.h"

namespace OC
{
	class Input final : public InputInterface
	{
	private:
		static Input* s_Instance; // Private singleton used when receiving input messages.

		HWND m_WindowHandle; // Handle to the window.
		WNDPROC m_OriginalWindowProcedure; // Pointer to the windows procedure function.
		int m_MouseX, m_MouseY; // Cursor position.
		int m_MousePrevX, m_MousePrevY; // Previous cursor position.
		int m_WheelDelta; // The change in mouse scroll-wheel position since last update.
		std::bitset<static_cast<unsigned int>(Key::_COUNT)> m_State; // The state of all keys and mouse buttons.
		std::bitset<static_cast<unsigned int>(Key::_COUNT)> m_PrevState; // The previous state of all keys and mouse buttons.
		bool m_StateChanged; // Tracks change in keys and mouse buttons since last update.
		bool m_MouseMoved; // Tracks change in mouse movements since last update.

		// Description: Handles input messages and passes them on to the window.
		// Parameters: 
		//    HWND _hWnd, handle to the window.
		//    UINT _message, the message code.
		//    WPARAM _wParam, additional data pertaining to the message.
		//    LPARAM _lParam, additional data pertaining to the message.
		// Returns: The repsonse to the message received.
		static LRESULT CALLBACK InputPocedure(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

		// Description: Sets a bit corresponding to a given key or mouse button input.
		// Parameters: 
		//    Key _key, the key or mouse button input whose state should be set.
		//    bool _isDown, if the input is pressed down.
		void Set(Key _key, bool _isDown);

		// Description: Sets a bit corresponding to a given key or mouse button input.
		// Parameters: 
		//    unsigned int _keyCode, the key or mouse button input whose state should be set.
		//    bool _isDown, if the input is pressed down.
		void Set(unsigned int _keyCode, bool _isDown);

		// Description: Updates how much the mouse wheel has moved.
		// Parameters: 
		//    int _delta, how much the wheel has moved.
		void SetWheel(int _delta);

		// Description: Updates this object's coordinates of the cursor.
		// Parameters: 
		//    int _x, the x position of the cursor.
		//    int _y, the y position of the cursor.
		void SetCursor(int _x, int _y);

	public:
		// Description: Constructs the input system and sets it up to receive input messages from the window.
		// Parameters: 
		//    const Window& _window, the window to intercept input messages from.
		Input(const Window& _window) :
			InputInterface(_window),
			m_MouseX(0), m_MouseY(0),
			m_MousePrevX(0), m_MousePrevY(0),
			m_WheelDelta(0),
			m_State(), m_PrevState(),
			m_StateChanged(false), m_MouseMoved(false)
		{
			assert(!s_Instance); // Error: There can only be one instance of Input.

			s_Instance = this;
			m_WindowHandle = static_cast<HWND>(_window.GetHandle());

			// Setup window message interception.
			m_OriginalWindowProcedure = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
				m_WindowHandle,
				GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(InputPocedure))
			);

			assert(m_OriginalWindowProcedure); // Error: Input will not receive input messages if this is NULL.
		}

		// Description: Input's cannot be created from other Input's.
		Input(const Input& _input) = delete;

		// Description: Remove the input from the window and clean up this instance.
		~Input()
		{
			// Stop intercepting messages from window.
			SetWindowLongPtr(
				m_WindowHandle,
				GWLP_WNDPROC,
				reinterpret_cast<LONG_PTR>(m_OriginalWindowProcedure)
			);

			s_Instance = nullptr;
		};

		// Description: Input's cannot be assigned to other Input's.
		virtual void operator=(const Input& _window) = delete;

		// Description: Returns if the given key state changed to pressed since last update.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key was just pressed.
		bool JustPressed(Key _key) const;

		// Description: Returns if the given key state changed to released since last update.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key was just released.
		bool JustReleased(Key _key) const;

		// Description: Returns if the given key is pressed.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key is pressed.
		bool Pressed(Key _key) const;

		// Description: Returns if the given key is released.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key is released.
		bool Released(Key _key) const;

		// Description: Gets the most recent position of the cursor.
		// Parameters: 
		//    int& _outX, the x position of the cursor.
		//    int& _outY, the y position of the cursor.
		void GetCursorPosition(int& _outX, int& _outY) const;

		// Description: Gets the relative motion of the cursor since last update.
		// Parameters: 
		//    int& _outDeltaX, relative motion on the x-axis.
		//    int& _outDeltaY, relative motion on the y-axis.
		void GetCursorDelta(int& _outDeltaX, int& _outDeltaY) const;

		// Description: Gets the change in mouse scroll-wheel position since last update.
		// Parameters: 
		//    int& _outDelta, change in position of the mouse wheel.
		void GetWheelDelta(int& _outDelta) const;

		// Description: Updates the state of the input system.
		void Update();
	};
}

#endif //defined(WIN32)
