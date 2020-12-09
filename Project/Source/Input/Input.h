/*
-------------------------------------------------------------------------------------------------------
	File: Input.h
	Author: Ozzie Mercado
	Created: December 7, 2020
	Modified: December 8, 2020
	Description: A cross-platform Input that can provide information about key states, cursor button
		states, and cursor position information. The class serves as a wrapper for a platform-specific 
		input implementation. Currently, only Win32 is supported.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

// Choose the appropriate platform input implementation.
#if defined(WIN32)
#include "Win32Input.h"
#else
static_assert(false, "Open Conquer Error: Input implementation not available for this platform");
#endif
