/*
-------------------------------------------------------------------------------------------------------
	File: InputInterface.h
	Author: Ozzie Mercado
	Created: December 7, 2020
	Modified: December 8, 2020
	Description: The interface that all input implementations share. Interface for getting information
		about mouse and key states.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

#include "../Window/Window.h"

namespace OC
{
	class InputInterface
	{
	public:
		// Description: Constructs the input system and sets it up to receive input messages from the window.
		// Parameters: 
		//    const Window& _window, the window to intercept input messages from.
		InputInterface(const Window& _window) {};

		// Description: Input's cannot be created from other Input's.
		InputInterface(const InputInterface& _input) = delete;

		// Description: Remove the input from the window and clean up this instance.
		virtual ~InputInterface() = default;

		// Description: Input's cannot be assigned to other Input's.
		virtual void operator=(const InputInterface& _window) = delete;

		// Description: Returns if the given key state changed to pressed since last update.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key was just pressed.
		virtual bool JustPressed(Key _key) const = 0;

		// Description: Returns if the given key state changed to released since last update.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key was just released.
		virtual bool JustReleased(Key _key) const = 0;

		// Description: Returns if the given key is pressed.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key is pressed.
		virtual bool Pressed(Key _key) const = 0;

		// Description: Returns if the given key is released.
		// Parameters: 
		//    Key _key, the key or mouse button input to check.
		// Returns: true, if the key is released.
		virtual bool Released(Key _key) const = 0;

		// Description: Gets the most recent position of the cursor.
		// Parameters: 
		//    int& _outX, the x position of the cursor.
		//    int& _outY, the y position of the cursor.
		virtual void GetCursorPosition(int& _outX, int& _outY) const = 0;

		// Description: Gets the relative motion of the cursor since last update.
		// Parameters: 
		//    int& _outDifX, relative motion on the x-axis.
		//    int& _outDifY, relative motion on the y-axis.
		virtual void GetCursorRelativeMovement(int& _outDifX, int& _outDifY) const = 0;

		// Description: Updates the state of the input system.
		virtual void Update() = 0;
	};
}
