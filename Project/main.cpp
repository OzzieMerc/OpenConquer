﻿/*
-------------------------------------------------------------------------------------------------------
	File: main.cpp
	Author: Ozzie Mercado
	Created: December 5, 2020
	Modified: December 7, 2020
	Description: Entry point for the application.
-------------------------------------------------------------------------------------------------------
*/

#include <iostream>
#include "Source/Window/Window.h"
#include "Source/Input/Input.h"
#include "Source/Renderer/Renderer.h"

int main(int _argc, char** _argv)
{
	OC::Window win(L"Open Conquer", 400, 200, 960, 600);
	OC::Input input(win);
	OC::Renderer renderer(win);
	
	while (true)
	{
		// Input
		input.Update(); // Input must be updated before window for JustPressed and JustReleased to work properly.
		if (!win.Update())
			break;

		// Logic
		if (input.JustPressed(OC::Key::A))
			std::cout << "Just Pressed: A\n";
		else if (input.JustReleased(OC::Key::A))
			std::cout << "Just Released: A\n";

		if (input.JustPressed(OC::Key::S))
			std::cout << "Just Pressed: S\n";
		else if (input.JustReleased(OC::Key::S))
			std::cout << "Just Released: S\n";
		else if (input.Pressed(OC::Key::S))
			std::cout << "Pressed: S\n";
		//else if (input.Released(OC::Key::S))
		//	std::cout << "Released: S\n"; // Commented out so it doesn't spam the console.

		static int x, y, difX, difY, wheelDelta;
		input.GetCursorPosition(x, y);
		input.GetCursorDelta(difX, difY);
		input.GetWheelDelta(wheelDelta);

		if (difX != 0 || difY != 0)
			printf("Mouse: x=%d y=%d dX=%d dY=%d\n", x, y, difX, difY);

		if (wheelDelta != 0)
			printf("Wheel: %d\n", wheelDelta);

		// Render
		renderer.Present();
	}

	std::cin.ignore();

	return 0;
}
