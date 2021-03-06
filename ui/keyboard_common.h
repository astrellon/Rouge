#pragma once

namespace am {
namespace ui {

	class Keyboard
	{
	public:
		enum Key 
		{
			KEY_NONE = 0,
			KEY_PAUSE = 1,
			KEY_INSERT = 2,
			KEY_PAGEUP = 3,
			KEY_PAGEDOWN = 4,
			KEY_END = 5,
			KEY_HOME = 6,
			KEY_DELETE = 7,
			KEY_BACKSPACE = 8,
			KEY_TAB = 9,
			KEY_ENTER = 10,
			KEY_UP_ARROW = 11,
			KEY_DOWN_ARROW = 12,
			KEY_LEFT_ARROW = 13,
			KEY_RIGHT_ARROW = 14,
			KEY_PRINT_SCREEN = 15,
			KEY_CAPS_LOCK = 16,
			KEY_LEFT_SHIFT = 17,
			KEY_RIGHT_SHIFT = 18,
			KEY_LEFT_CTRL = 19,
			KEY_RIGHT_CTRL = 20,
			KEY_LEFT_ALT = 21,
			KEY_RIGHT_ALT = 22,
			KEY_SCROLL_LOCK = 23,
			KEY_LEFT_SPECIAL = 24,
			KEY_RIGHT_SPECIAL = 25,
			KEY_RESERVED_2 = 26,
			KEY_ESC = 27,
			KEY_SHIFT = 28,
			KEY_CTRL = 29,
			KEY_ALT = 30,
			KEY_RESERVED_1 = 31,
			KEY_SPACE = 32,
			KEY_EXCLAIMATION = 33,
			KEY_DOUBLE_QUOTE = 34,
			KEY_HASH = 35,
			KEY_DOLLAR = 36,
			KEY_PERCENT = 37,
			KEY_AMPERSAND = 38,
			KEY_SINGLE_QUOTE = 39,
			KEY_OPEN_BRACKET = 40,
			KEY_CLOSE_BRACKET = 41,
			KEY_STAR = 42,
			KEY_PLUS = 43,
			KEY_COMMA = 44,
			KEY_MINUS = 45,
			KEY_PERIOD = 46,
			KEY_FORWARD_SLASH = 47,
			KEY_0 = 48,
			KEY_1 = 49,
			KEY_2 = 50,
			KEY_3 = 51,
			KEY_4 = 52,
			KEY_5 = 53,
			KEY_6 = 54,
			KEY_7 = 55,
			KEY_8 = 56,
			KEY_9 = 57,
			KEY_COLON = 58,
			KEY_SEMI_COLON = 59,
			KEY_OPEN_ANGLE_BRACKET = 60,
			KEY_EQUALS = 61,
			KEY_CLOSE_ANGLE_BRACKET = 62,
			KEY_QUESTION = 63,
			KEY_AT = 64,
			KEY_A = 65,
			KEY_B = 66,
			KEY_C = 67,
			KEY_D = 68,
			KEY_E = 69,
			KEY_F = 70,
			KEY_G = 71,
			KEY_H = 72,
			KEY_I = 73,
			KEY_J = 74,
			KEY_K = 75,
			KEY_L = 76,
			KEY_M = 77,
			KEY_N = 78,
			KEY_O = 79,
			KEY_P = 80,
			KEY_Q = 81,
			KEY_R = 82,
			KEY_S = 83,
			KEY_T = 84,
			KEY_U = 85,
			KEY_V = 86,
			KEY_W = 87,
			KEY_X = 88,
			KEY_Y = 89,
			KEY_Z = 90,
			KEY_OPEN_SQUARE_BRACKET = 91,
			KEY_BACK_SLASH = 92,
			KEY_CLOSE_SQUARE_BRACKET = 93,
			KEY_HAT = 94,
			KEY_UNDERSCORE = 95,
			KEY_BACKTICK = 96,
			KEY_a = 97,
			KEY_b = 98,
			KEY_c = 99,
			KEY_d = 100,
			KEY_e = 101,
			KEY_f = 102,
			KEY_g = 103,
			KEY_h = 104,
			KEY_i = 105,
			KEY_j = 106,
			KEY_k = 107,
			KEY_l = 108,
			KEY_m = 109,
			KEY_n = 110,
			KEY_o = 111,
			KEY_p = 112,
			KEY_q = 113,
			KEY_r = 114,
			KEY_s = 115,
			KEY_t = 116,
			KEY_u = 117,
			KEY_v = 118,
			KEY_w = 119,
			KEY_x = 120,
			KEY_y = 121,
			KEY_z = 122,
			KEY_OPEN_CURLY_BRACKET = 123,
			KEY_PIPE = 124,
			KEY_CLOSE_CURCLY_BRACKET = 125,
			KEY_GRAVE = 126,
			KEY_NUM_0 = 127,
			KEY_NUM_1 = 128,
			KEY_NUM_2 = 129,
			KEY_NUM_3 = 130,
			KEY_NUM_4 = 131,
			KEY_NUM_5 = 132,
			KEY_NUM_6 = 133,
			KEY_NUM_7 = 134,
			KEY_NUM_8 = 135,
			KEY_NUM_9 = 136,
			KEY_NUM_PLUS = 137,
			KEY_NUM_MINUS = 138,
			KEY_NUM_MULTIPLY = 139,
			KEY_NUM_DIVIDE = 140,
			KEY_F1 = 141,
			KEY_F2 = 142,
			KEY_F3 = 143,
			KEY_F4 = 144,
			KEY_F5 = 145,
			KEY_F6 = 146,
			KEY_F7 = 147,
			KEY_F8 = 148,
			KEY_F9 = 149,
			KEY_F10 = 150,
			KEY_F11 = 151,
			KEY_F12 = 152,
			KEY_MAX_LENGTH = 153
		};

		enum EventType 
		{
			KEY_DOWN, KEY_UP, KEY_PRESS
		};

		static const char *EventTypeNames[];
		static const char *KeyNames[];

		static Key getKey(int key);
		static const char *getKeyName(Key key);
	};

}
}
