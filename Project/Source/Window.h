/*
-------------------------------------------------------------------------------------------------------
	File: Window.h
	Author: Ozzie Mercado
	Created: December 6, 2020
	Modified: December 7, 2020
	Description: A cross-platform Window that can open, close, and update a window. The class serves as
		a wrapper for a platform-specific window implementation. Currently, only Win32 is supported.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

// Choose the appropriate platform window implementation.
#if defined(WIN32)
#include "Win32Window.h"
#else
static_assert(false, "Open Conquer Error: Window implentation not available for this platform");
#endif
