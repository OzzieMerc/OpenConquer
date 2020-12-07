/*
-------------------------------------------------------------------------------------------------------
	File: main.cpp
	Author: Ozzie Mercado
	Created: December 5, 2020
	Modified: December 7, 2020
	Description: Entry point for the application.
-------------------------------------------------------------------------------------------------------
*/

#include <iostream>
#include "Source\Window.h"

int main(int _argc, char** _argv)
{
	OC::Window win(L"Open Conquer", 400, 200, 960, 600);
	
	while (win.Update())
	{
		// Update stuff.
	}

	std::cin.ignore();

	return 0;
}
