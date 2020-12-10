/*
-------------------------------------------------------------------------------------------------------
	File: Renderer.h
	Author: Ozzie Mercado
	Created: December 9, 2020
	Modified: December 10, 2020
	Description: A cross-platform renderer. The class serves as a wrapper for a platform-specific 
				renderer implementation. Currently, only Win32 is supported.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

// Choose the appropriate platform renderer implementation.
#if defined(WIN32)
#include "Win32DirectX11Renderer.h"
#else
static_assert(false, "Open Conquer Error: Renderer implementation not available for this platform");
#endif
