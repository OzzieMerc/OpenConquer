/*
-------------------------------------------------------------------------------------------------------
	File: Win32Keys.h
	Author: Ozzie Mercado
	Created: December 7, 2020
	Modified: December 8, 2020
	Description: The Win32 key and mouse button mappings.
-------------------------------------------------------------------------------------------------------
*/

#pragma once

#if defined(WIN32)

namespace OC
{
	enum class Key : unsigned char
	{
		MOUSE_LEFT = 1U,
		MOUSE_RIGHT = 2U,
		MOUSE_MIDDLE = 4U,
		MOUSE_X1 = 5U,
		MOUSE_X2 = 6U,
		BACK = 8U,
		TAB = 9U,
		RETURN = 13U,
		SHIFT = 16U,
		CONTROL = 17U,
		CAPITAL = 20U,
		ESCAPE = 27U,
		SPACE = 32U,
		END = 35U,
		HOME = 36U,
		ARROW_LEFT = 37U,
		ARROW_UP = 38U,
		ARROW_RIGHT = 39U,
		ARROW_DOWN = 40U,
		INSERT = 45U,
		DEL = 46U,
		HELP = 47U,
		NUMROW_0 = 48U,
		NUMROW_1 = 49U,
		NUMROW_2 = 50U,
		NUMROW_3 = 51U,
		NUMROW_4 = 52U,
		NUMROW_5 = 53U,
		NUMROW_6 = 54U,
		NUMROW_7 = 55U,
		NUMROW_8 = 56U,
		NUMROW_9 = 57U,
		A = 65U,
		B = 66U,
		C = 67U,
		D = 68U,
		E = 69U,
		F = 70U,
		G = 71U,
		H = 72U,
		I = 73U,
		J = 74U,
		K = 75U,
		L = 76U,
		M = 77U,
		N = 78U,
		O = 79U,
		P = 80U,
		Q = 81U,
		R = 82U,
		S = 83U,
		T = 84U,
		U = 85U,
		V = 86U,
		W = 87U,
		X = 88U,
		Y = 89U,
		Z = 90U,
		NUMPAD_0 = 96U,
		NUMPAD_1 = 97U,
		NUMPAD_2 = 98U,
		NUMPAD_3 = 99U,
		NUMPAD_4 = 100U,
		NUMPAD_5 = 101U,
		NUMPAD_6 = 102U,
		NUMPAD_7 = 103U,
		NUMPAD_8 = 104U,
		NUMPAD_9 = 105U,
		MULTIPLY = 106U,
		ADD = 107U,
		SEPARATOR = 108U,
		SUBTRACT = 109U,
		DECIMAL = 110U,
		DIVIDE = 111U,
		F1 = 112U,
		F2 = 113U,
		F3 = 114U,
		F4 = 115U,
		F5 = 116U,
		F6 = 117U,
		F7 = 118U,
		F8 = 119U,
		F9 = 120U,
		F10 = 121U,
		F11 = 122U,
		F12 = 123U,
		LSHIFT = 160U,
		RSHIFT = 161U,
		LCONTROL = 162U,
		RCONTROL = 163U,
		_COUNT, // Used for looping or array allocating.
	};
}

#endif //defined(WIN32)
