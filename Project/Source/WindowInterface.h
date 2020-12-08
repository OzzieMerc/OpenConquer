/*
-------------------------------------------------------------------------------------------------------
	File: WindowInterface.h
	Author: Ozzie Mercado
	Created: December 6, 2020
	Modified: December 7, 2020
	Description: The interface that all window implementations share. Interface for opening, closing, 
	             and updating a window.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

namespace OC
{
	class WindowInterface
	{
	protected:
		const wchar_t* m_Name; // The name of the window.
		int m_X, m_Y; // The position of the window.
		unsigned int m_Width, m_Height; // The size of the client view area of the window.

	public:
		// Description: Constructs the window and opens it.
		// Parameters: 
		//    const wchar_t* _name, the text to place in the caption area (titlebar) of the window.
		//    int _x, the x position of the window.
		//    int _y, the y position of the window.
		//    unsigned int _width, the horizontal width of the drawable area of the window.
		//    unsigned int _height, the vertical height of the drawable area of the window.
		WindowInterface(const wchar_t* _name, int _x, int _y, unsigned int _width, unsigned int _height) :
			m_Name(_name),
			m_X(_x), m_Y(_y),
			m_Width(_width), m_Height(_height)
		{}

		// Description: Window's cannot be created from other window's.
		WindowInterface(const WindowInterface& _window) = delete;

		// Description: Closes the window if it's open, and cleans up references.
		virtual ~WindowInterface()
		{
			m_Name = nullptr;
		}

		// Description: Window's cannot be assigned to other window's.
		virtual void operator=(const WindowInterface& _window) = delete;

		// Description: Opens a window.
		// Returns: true, if the window opened.
		virtual bool Open() = 0;

		// Description: Closes the window.
		virtual void Close() = 0;

		// Description: Runs the window message loop.
		// Returns: false, if the window is closed.
		virtual bool Update() = 0;

		// Description: Returns a handle to the window.
		// Returns: Pointer to the window handle.
		virtual void* GetHandle() const = 0;
	};
}
