/*
-------------------------------------------------------------------------------------------------------
	File: Win32Input.cpp
	Author: Ozzie Mercado
	Created: December 7, 2020
	Modified: December 8, 2020
	Note: See header for more documentation.
-------------------------------------------------------------------------------------------------------
*/

#include "Win32Input.h"

namespace OC
{
	// private

	Input* Input::s_Instance = nullptr;

	LRESULT Input::InputPocedure(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
	{
		assert(s_Instance); // Error: How are we even in here if there is no instance?

		switch (_message)
		{
		case WM_KEYDOWN:		s_Instance->Set(static_cast<unsigned int>(_wParam), true);	break;
		case WM_KEYUP:			s_Instance->Set(static_cast<unsigned int>(_wParam), false);	break;
		case WM_LBUTTONDOWN:	s_Instance->Set(Key::MOUSE_LEFT, true);						break;
		case WM_LBUTTONUP:		s_Instance->Set(Key::MOUSE_LEFT, false);					break;
		case WM_RBUTTONDOWN:	s_Instance->Set(Key::MOUSE_RIGHT, true);					break;
		case WM_RBUTTONUP:		s_Instance->Set(Key::MOUSE_RIGHT, false);					break;
		case WM_MBUTTONDOWN:	s_Instance->Set(Key::MOUSE_MIDDLE, true);					break;
		case WM_MBUTTONUP:		s_Instance->Set(Key::MOUSE_MIDDLE, false);					break;
		case WM_MOUSEWHEEL:
		{
			// TODO: Test this on a freely-rotating wheel. May have to consider using float to represent m_WheelDelta.
			short delta = GET_WHEEL_DELTA_WPARAM(_wParam) / WHEEL_DELTA;
			s_Instance->SetWheel(static_cast<int>(delta));
			break;
		}
		case WM_MOUSEMOVE:
		{
			POINTS cursor = MAKEPOINTS(_lParam);
			s_Instance->SetCursor(static_cast<int>(cursor.x), static_cast<int>(cursor.y));
			break;
		}
		}

		return CallWindowProc(s_Instance->m_OriginalWindowProcedure, _hWnd, _message, _wParam, _lParam);
	}

	void Input::Set(Key _key, bool _isDown)
	{
		assert(_key != Key::_COUNT); // _COUNT is not a valid key.

		unsigned int bit = static_cast<unsigned int>(_key); // The bit location of the key state.

		if (m_State.test(bit) != _isDown)
		{
			m_State.set(bit, _isDown); // Set the key state bit.
			m_StateChanged = true;
		}
	}

	void Input::Set(unsigned int _keyCode, bool _isDown)
	{
		// Ignore key codes outside of what we're expecting.
		if (_keyCode < static_cast<unsigned int>(Key::_COUNT))
		{
			if (m_State.test(_keyCode) != _isDown)
			{
				m_State.set(_keyCode, _isDown);
				m_StateChanged = true;
			}
		}
	}

	void Input::SetWheel(int _delta)
	{
		m_WheelDelta = _delta;
		m_MouseMoved = true;
	}

	void Input::SetCursor(int _x, int _y)
	{
		m_MouseX = _x;
		m_MouseY = _y;
		m_MouseMoved = true;
	}

	// public

	Input::Input(const Window& _window) :
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

	Input::~Input()
	{
		// Stop intercepting messages from window.
		SetWindowLongPtr(
			m_WindowHandle,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(m_OriginalWindowProcedure)
		);

		s_Instance = nullptr;
	};

	bool Input::JustPressed(Key _key) const
	{
		assert(_key != Key::_COUNT); // _COUNT is not a valid key.

		unsigned int bit = static_cast<unsigned int>(_key); // The bit location of the key state.

		// Has the key state changed since last update?
		return m_State.test(bit) == true &&
			   m_PrevState.test(bit) == false;
	}

	bool Input::JustReleased(Key _key) const
	{
		assert(_key != Key::_COUNT); // _COUNT is not a valid key.

		const unsigned int bit = static_cast<unsigned int>(_key);

		return m_PrevState.test(bit) == true &&
			   m_State.test(bit) == false;
	}

	bool Input::Pressed(Key _key) const
	{
		assert(_key != Key::_COUNT); // _COUNT is not a valid key.

		return m_State.test(static_cast<unsigned int>(_key));
	}

	bool Input::Released(Key _key) const
	{
		assert(_key != Key::_COUNT); // _COUNT is not a valid key.

		return !m_State.test(static_cast<unsigned int>(_key));
	}

	void Input::GetCursorPosition(int& _outX, int& _outY) const
	{
		_outX = m_MouseX;
		_outY = m_MouseY;
	}

	void Input::GetCursorDelta(int& _outDeltaX, int& _outDeltaY) const
	{
		_outDeltaX = m_MouseX - m_MousePrevX;
		_outDeltaY = m_MouseY - m_MousePrevY;
	}

	void Input::GetWheelDelta(int& _outDelta) const
	{
		_outDelta = m_WheelDelta;
	}

	void Input::Update()
	{
		// If an input key state changed, copy the current state.
		if (m_StateChanged)
		{
			m_PrevState = m_State;
			m_StateChanged = false;
		}

		if (m_MouseMoved)
		{
			// Keep track of previous mouse position for relative movement calculation.
			m_MousePrevX = m_MouseX;
			m_MousePrevY = m_MouseY;
			m_WheelDelta = 0;
			m_MouseMoved = false;
		}
	}
}